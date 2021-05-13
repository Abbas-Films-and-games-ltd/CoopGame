// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/COOP_GameMode_Gameplay.h"
#include "COOP_GameState.h"
#include "COOP_PlayerState.h"
#include "EngineUtils.h"
#include "Components/FPSHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CoopGame/Public/AI/TrackerBot.h"

ACOOP_GameMode_Gameplay::ACOOP_GameMode_Gameplay()
{
	GameStateClass = ACOOP_GameState::StaticClass();
	PlayerStateClass = ACOOP_PlayerState::StaticClass();

	TimerBetweenWaves = 2.0f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;

	bIsLobby = false;
}

void ACOOP_GameMode_Gameplay::StartPlay()
{
	Super::StartPlay();

	if (!bIsLobby)
	{
		PrepareNextWave();
	}

}

void ACOOP_GameMode_Gameplay::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsLobby)
	{
		CheckWaveState();
		CheckAnyPlayerAlive();
	}

}

void ACOOP_GameMode_Gameplay::StartWave()
{
	WaveCount++;
	NrOfBotsToSpawn = NrBotsMultiplier * WaveCount;
	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ACOOP_GameMode_Gameplay::SpawnBotTimerElapsed, 1.0f, true, 0.0f);

	SetWaveState(EWaveState::WaveInProgress);
}

void ACOOP_GameMode_Gameplay::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
	SetWaveState(EWaveState::WaitingToComplete);
}

void ACOOP_GameMode_Gameplay::PrepareNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWave, this, &ACOOP_GameMode_Gameplay::StartWave, TimerBetweenWaves, false);
	SetWaveState(EWaveState::WaitingToStart);
	RespawnDeadPlayers();
}

void ACOOP_GameMode_Gameplay::CheckWaveState()
{
	bool bIsPreparingWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWave);

	if (NrOfBotsToSpawn > 0 || bIsPreparingWave)
	{
		return;
	}

	bool bIsAnyBotAlive = false;

	for (TActorIterator<APawn> CharacterItr(GetWorld()); CharacterItr; ++CharacterItr)
	{
		APawn* TestPawn = *CharacterItr;

		if (TestPawn == nullptr || TestPawn->IsPlayerControlled()) {
			continue;
		}
		UFPSHealthComponent* HealthComp = Cast<UFPSHealthComponent>(TestPawn->GetComponentByClass(UFPSHealthComponent::StaticClass()));
		if (HealthComp && HealthComp->GetHealth() > 0.0f)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	if (!bIsAnyBotAlive)
	{
		PrepareNextWave();
	}
}

void ACOOP_GameMode_Gameplay::CheckAnyPlayerAlive()
{
	bool bAllPlayersAlive = true;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* PlayerPawn = PC->GetPawn();
			UFPSHealthComponent* HealthComp = Cast<UFPSHealthComponent>(PlayerPawn->GetComponentByClass(UFPSHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				return;
			}
		}
	}

	GameOver();
}

void ACOOP_GameMode_Gameplay::GameOver()
{
	EndWave();
	//UE_LOG(LogTemp, Warning, TEXT("GAME OVER!!"));
	SetWaveState(EWaveState::GameOver);
	
	GoToNextLevel();
}

void ACOOP_GameMode_Gameplay::SetWaveState(EWaveState NewState)
{
	ACOOP_GameState* GS = GetGameState<ACOOP_GameState>();
	if (ensureAlways(GS))
	{
		GS->SetWaveState(NewState);
		CurrentWaveState = NewState;
	}
}

void ACOOP_GameMode_Gameplay::RespawnDeadPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}
}

void ACOOP_GameMode_Gameplay::SpawnBotTimerElapsed()
{
	SpawnNewBot();
	NrOfBotsToSpawn--;
	if (NrOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}
