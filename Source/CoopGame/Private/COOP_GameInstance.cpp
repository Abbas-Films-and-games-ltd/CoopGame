// Fill out your copyright notice in the Description page of Project Settings.


#include "COOP_GameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "COOP_GameMode_Base.h"
#include "MenuSystem/COOP_MainMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "MenuSystem/COOP_PauseMenu.h"

const FName CURRENT_SESSION = NAME_GameSession; /*TEXT("Game");*/

UCOOP_GameInstance::UCOOP_GameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWBPClass(TEXT("/Game/Blueprints/UI/WBP_MainMenu"));
	if (!ensure(MainMenuWBPClass.Class != nullptr)) return;
	MainMenuClass = MainMenuWBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuWBPClass(TEXT("/Game/Blueprints/UI/WBP_PauseMenu"));
	if (!ensure(PauseMenuWBPClass.Class != nullptr)) return;
	PauseMenuClass = PauseMenuWBPClass.Class;

	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = 1;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bUsesPresence = true;
}

void UCOOP_GameInstance::Init()
{	
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Subsystem name is: %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Session found"));
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCOOP_GameInstance::OnCreateNewSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCOOP_GameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCOOP_GameInstance::OnJoinSessionComplete);
			//SessionInterface->OnSessionFailureDelegates.AddUObject(this, &UCOOP_GameInstance::OnSessionLost);
			//SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCOOP_GameInstance::OnDestroySessionsComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Subsystem found!"));
	}
}

void UCOOP_GameInstance::HostGame()
{
	if (SessionInterface.IsValid())
	{
		if (SessionInterface->GetNamedSession(CURRENT_SESSION) != nullptr)
		{
			SessionInterface->DestroySession(CURRENT_SESSION);
			UE_LOG(LogTemp, Error, TEXT("Session DELETED!"));
		}
		
		/*@TODO: Set a proper settings menu =========
		//==========================================*/
		ACOOP_GameMode_Base* CurrentGameMode = Cast<ACOOP_GameMode_Base>(GetWorld()->GetAuthGameMode());
		if (CurrentGameMode)
		{
			SessionSettings = CurrentGameMode->GameSessionSettings;
		}
		
		/*SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 4;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bIsDedicated = false;
		SessionSettings.bUsesPresence = true;*/
		
		SessionInterface->CreateSession(0, CURRENT_SESSION, SessionSettings);
	}
}

void UCOOP_GameInstance::OnCreateNewSessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("HOSTING GAME"));

		if (MainMenu != nullptr)
		{
			MainMenu->Teardown();
		}
		GetWorld()->ServerTravel("/Game/Maps/Lobby?listen");
	}
}

void UCOOP_GameInstance::FindNewSessions()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->bIsLanQuery = true;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		
		UE_LOG(LogTemp, Warning, TEXT("Finding sessions!"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UCOOP_GameInstance::OnFindSessionsComplete(bool Success)
{
	auto& SessionSearchResults = SessionSearch->SearchResults;
	int32 NumPlayersSlots = 0;
	int32 NumMaxPlayers = 0;
	if (SessionSearchResults.Num() > 0)
	{
		TArray<FString> ServerList;
		for (int32 i = 0; i < SessionSearchResults.Num(); i++)
		{
			ServerList.AddUnique(*SessionSearchResults[i].GetSessionIdStr());
			NumMaxPlayers = SessionSearchResults[i].Session.SessionSettings.NumPublicConnections;
			NumPlayersSlots = NumMaxPlayers - SessionSearchResults[i].Session.NumOpenPublicConnections;
			FString ServerUserName = SessionSearchResults[i].Session.OwningUserName;
			MainMenu->AddEntityToServerList(ServerList, NumPlayersSlots, NumMaxPlayers, ServerUserName);
			UE_LOG(LogTemp, Warning, TEXT("Available session id: %s"), *SessionSearchResults[i].GetSessionIdStr());
		}
		
		
	}
	UE_LOG(LogTemp, Warning, TEXT("Session search complete!"));
}

void UCOOP_GameInstance::JoinGame(int32 ServerIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Joinging Server with index %i"), ServerIndex);
	if(!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;
	
	if (MainMenu != nullptr)
	{
		MainMenu->Teardown();
	}

	SessionInterface->JoinSession(0, CURRENT_SESSION, SessionSearch->SearchResults[ServerIndex]);
}

void UCOOP_GameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;
	
	FString ServerToJoin;
	if (!SessionInterface->GetResolvedConnectString(SessionName, ServerToJoin))
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectString and/ServerToJoin are not valid"));
		return;
	}
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController != nullptr)) return;
	
	PlayerController->ClientTravel(ServerToJoin, ETravelType::TRAVEL_Absolute);
}

void UCOOP_GameInstance::OnDestroySessionsComplete()
{
	ClientTravelToLevel(MainMenuLevel);
}

void UCOOP_GameInstance::LoadMenu()
{
	if (MainMenuClass == nullptr) { return; }
	MainMenu = CreateWidget<UCOOP_MainMenu>(this, MainMenuClass);
	if (MainMenu != nullptr)
	{
		MainMenu->Setup();
		MainMenu->SetMenuInterface(this);
	}
}

void UCOOP_GameInstance::LoadPauseMenu()
{
	if (PauseMenuClass == nullptr) { return; }
	PauseMenu = CreateWidget<UCOOP_PauseMenu>(this, PauseMenuClass);
	if (PauseMenu != nullptr)
	{
		PauseMenu->Setup();
		PauseMenu->SetMenuInterface(this);
	}
}

void UCOOP_GameInstance::ClientTravelToLevel(const FString& LevelToTravel)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr) {	return;	}
	PlayerController->ClientTravel(*LevelToTravel, ETravelType::TRAVEL_Absolute);
}

void UCOOP_GameInstance::OnSessionLost()
{
	SessionInterface->DestroySession(CURRENT_SESSION);
}

