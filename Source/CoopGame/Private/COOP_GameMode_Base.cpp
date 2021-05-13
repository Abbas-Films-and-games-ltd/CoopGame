// Fill out your copyright notice in the Description page of Project Settings.


#include "COOP_GameMode_Base.h"


ACOOP_GameMode_Base::ACOOP_GameMode_Base()
{
	GameSessionSettings.NumPublicConnections = MaxPlayers;
	GameSessionSettings.bIsLANMatch = bIsLANMatch;
	GameSessionSettings.bShouldAdvertise = bIsPublicGame;
	GameSessionSettings.bIsDedicated = bIsDedicatedServer;
	GameSessionSettings.bUsesPresence = bUseLobbySystem;
}

void ACOOP_GameMode_Base::PostLogin(APlayerController* NewPlayer)
{
	CountInGamePlayers(NewPlayer);
	GoToNextLevel();
}

void ACOOP_GameMode_Base::Logout(AController* Exiting)
{
	if (AllPlayers.Contains(Exiting))
	{
		int32 const PlayerIndex = AllPlayers.Find(Exiting);
		AllPlayers.RemoveAt(PlayerIndex);
	}

	CountInGamePlayers(Exiting);
}

int32 ACOOP_GameMode_Base::CountInGamePlayers(AController* InNewPlayer)
{
	if(!InNewPlayer->IsPlayerController())
	{
		return PlayersCount;
	}
	
	AllPlayers.AddUnique(InNewPlayer);
	PlayersCount = AllPlayers.Num();
	UE_LOG(LogTemp, Warning, TEXT("Current logged player count is: %i"), PlayersCount);
	if(PlayersCount >= 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Max Players count reached: %i"), PlayersCount);
	}

	return PlayersCount;
}

void ACOOP_GameMode_Base::GoToNextLevel()
{
	
	if (bIsLobby)
	{
		if (PlayersCount >= MaxPlayers)
		{
			bReady = true;
		}
	}

	if (bReady)
	{
		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;

		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/Maps/%s?listen"), * NextLevelName;
	}
}
