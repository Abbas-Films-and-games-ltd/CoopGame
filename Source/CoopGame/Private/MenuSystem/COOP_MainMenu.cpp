// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/COOP_MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MenuSystem/ServerListEntity.h"
#include "UObject/ConstructorHelpers.h"


UCOOP_MainMenu::UCOOP_MainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerListEntityClass(TEXT("/Game/Blueprints/UI/WBP_ServerListEntity"));
	if (!ensure(ServerListEntityClass.Class != nullptr)) return;
	ServerListEntity = ServerListEntityClass.Class;
}

bool UCOOP_MainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }
	if (Host_BTN == nullptr){return false;	}
	Host_BTN->OnClicked.AddDynamic(this, &UCOOP_MainMenu::HostGame);
	if (Join_Menu_BTN == nullptr) { return false; }
	Join_Menu_BTN->OnClicked.AddDynamic(this, &UCOOP_MainMenu::OpenJoinMenu);
	if (Refresh_BTN == nullptr) { return false; }
	Refresh_BTN->OnClicked.AddDynamic(this, &UCOOP_MainMenu::RefreshServerList);
	if (Back_BTN == nullptr) { return false; }
	Back_BTN->OnClicked.AddDynamic(this, &UCOOP_MainMenu::OpenMainMenu);
	if (Exit_BTN == nullptr) { return false; }
	Exit_BTN->OnClicked.AddDynamic(this, &UCOOP_MainMenu::ExitGame);

	return true;
}

void UCOOP_MainMenu::ExitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UCOOP_MainMenu::HostGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Hosing Game!!"));
	if (MenuInterface != nullptr)
	{
		MenuInterface->HostGame();
	}
}

void UCOOP_MainMenu::JoinGame(int32 ServerIndex)
{
	if (ServerIndex == NULL)
	{
		ServerIndex = 0;
	}
	if (MenuInterface != nullptr)
	{
		MenuInterface->JoinGame(ServerIndex);
	}
}

void UCOOP_MainMenu::RefreshServerList()
{
	MenuInterface->FindNewSessions();
}

//void UCOOP_MainMenu::SelectIndex(int32 Index)
//{
//	SelectedIndex = Index;
//}

void UCOOP_MainMenu::OpenJoinMenu()
{
	WidgetSwitch->SetActiveWidget(JoinMenu_VBox);
	RefreshServerList();
}

void UCOOP_MainMenu::AddEntityToServerList(TArray<FString> ServerNames, int32 NumAvaiableSlots, int32 MaxPlayerAllowed, FString ServerOwnerName)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;
	ServersListNames = ServerNames;
	ServerListSBox->ClearChildren();
	for(int32 ServerIndex = 0; ServerIndex < ServersListNames.Num(); ++ServerIndex)
		//for (const FString& ServerName : ServerNames)
	{
		UServerListEntity* ServerEntity = CreateWidget<UServerListEntity>(World, ServerListEntity);
		if (!ensure(ServerEntity != nullptr)) return;
		FString ServerName = ServerOwnerName;
			//ServersListNames[ServerIndex];
		
		//ServerEntity->ServerName_TXT->SetText(FText::FromString(ServerName));
		ServerEntity->SetupEntity(this, ServerIndex, &ServerName, NumAvaiableSlots, MaxPlayerAllowed);
		
		//ServerEntity->ServerIndex = ServerIndex;
		//ServerEntity->MainMenu = this;
		
		ServerListSBox->AddChild(ServerEntity);
	}
}

void UCOOP_MainMenu::OpenMainMenu()
{
	WidgetSwitch->SetActiveWidget(MainMenu_VBox);
}
