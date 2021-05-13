// Fill out your copyright notice in the Description page of Project Settings.


#include "COOP_GameMode.h"
//#include "COOP_GameState.h"
//#include "COOP_PlayerState.h"
//#include "EngineUtils.h"
//#include "Components/FPSHealthComponent.h"
//#include "Kismet/GameplayStatics.h"
//#include "CoopGame/Public/AI/TrackerBot.h"
//
//
//ACOOP_GameMode::ACOOP_GameMode()
//{
//	GameStateClass = ACOOP_GameState::StaticClass();
//	PlayerStateClass = ACOOP_PlayerState::StaticClass();
//
//	TimerBetweenWaves = 2.0f;
//
//	PrimaryActorTick.bCanEverTick = true;
//	PrimaryActorTick.TickInterval = 1.0f;
//
//}
//
//void ACOOP_GameMode::StartPlay()
//{
//	Super::StartPlay();
//	
//	if (!bIsLobby)
//	{
//		PrepareNextWave();
//	}
//	
//}
//
//void ACOOP_GameMode::Tick(float DeltaSeconds)
//{
//	Super::Tick(DeltaSeconds);
//
//	if (!bIsLobby)
//	{
//		CheckWaveState();
//		CheckAnyPlayerAlive();
//	}
//	
//}
//
//void ACOOP_GameMode::StartWave()
//{
//	WaveCount++;
//	NrOfBotsToSpawn = NrBotsMultiplier * WaveCount;
//	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ACOOP_GameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);
//
//	SetWaveState(EWaveState::WaveInProgress);
//}
//
//void ACOOP_GameMode::EndWave()
//{
//	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
//	SetWaveState(EWaveState::WaitingToComplete);
//}
//
//void ACOOP_GameMode::PrepareNextWave()
//{
//	GetWorldTimerManager().SetTimer(TimerHandle_NextWave, this, &ACOOP_GameMode::StartWave, TimerBetweenWaves, false);
//	SetWaveState(EWaveState::WaitingToStart);
//	RespawnDeadPlayers();
//}
//
//void ACOOP_GameMode::CheckWaveState()
//{
//	bool bIsPreparingWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWave);
//
//	if (NrOfBotsToSpawn > 0 || bIsPreparingWave)
//	{
//		return;
//	}
//
//	bool bIsAnyBotAlive = false;
//
//	for (TActorIterator<APawn> CharacterItr(GetWorld()); CharacterItr; ++CharacterItr) 
//	{
//			APawn* TestPawn = *CharacterItr;
//
//			if (TestPawn == nullptr || TestPawn->IsPlayerControlled()) {
//				continue;
//			}
//		UFPSHealthComponent* HealthComp = Cast<UFPSHealthComponent>(TestPawn->GetComponentByClass(UFPSHealthComponent::StaticClass()));
//		if (HealthComp && HealthComp->GetHealth() > 0.0f)
//		{
//			bIsAnyBotAlive = true;
//			break;
//		}
//	}
//
//	if (!bIsAnyBotAlive)
//	{
//		PrepareNextWave();
//	}
//}
//
//void ACOOP_GameMode::CheckAnyPlayerAlive()
//{
//	bool bAllPlayersAlive = true;
//
//	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
//	{
//		APlayerController* PC = It->Get();
//		if (PC && PC->GetPawn())
//		{
//			APawn* PlayerPawn = PC->GetPawn();
//			UFPSHealthComponent* HealthComp = Cast<UFPSHealthComponent>(PlayerPawn->GetComponentByClass(UFPSHealthComponent::StaticClass()));
//			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
//			{
//				return;
//			}
//		}
//	}
//	
//	GameOver();
//}
//
//void ACOOP_GameMode::GameOver()
//{
//	EndWave();
//	//UE_LOG(LogTemp, Warning, TEXT("GAME OVER!!"));
//	SetWaveState(EWaveState::GameOver);
//}
//
//void ACOOP_GameMode::SetWaveState(EWaveState NewState)
//{
//	ACOOP_GameState* GS = GetGameState<ACOOP_GameState>();
//	if (ensureAlways (GS))
//	{
//		GS->SetWaveState(NewState);
//		CurrentWaveState = NewState;
//	}
//}
//
//void ACOOP_GameMode::RespawnDeadPlayers()
//{
//	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
//	{
//		APlayerController* PC = It->Get();
//		if (PC && PC->GetPawn() == nullptr)
//		{
//			RestartPlayer(PC);
//		}
//	}
//}
//
//void ACOOP_GameMode::SpawnBotTimerElapsed()
//{
//	SpawnNewBot();
//	NrOfBotsToSpawn--;
//	if (NrOfBotsToSpawn <= 0)
//	{
//		EndWave();
//	}
//}
//
//void ACOOP_GameMode::PostLogin(APlayerController* NewPlayer)
//{
//	Super::PostLogin(NewPlayer);
//	
//	AllPlayers.AddUnique(NewPlayer);
//
//	CountInGamePlayers();
//
//	if (PlayersCount >= MaxPlayers)
//	{
//		if (bIsLobby)
//		{
//			UWorld* World = GetWorld();
//			if (!ensure(World != nullptr)) return;
//
//			bUseSeamlessTravel = true;
//			World->ServerTravel("/Game/Maps/%s?listen"), *LevelName;
//		}
//	}
//}
//
//void ACOOP_GameMode::Logout(AController* Exiting)
//{
//	Super::Logout(Exiting);
//	
//	if (AllPlayers.Contains(Exiting))
//	{
//		int32 const PlayerIndex = AllPlayers.Find(Exiting);
//		AllPlayers.RemoveAt(PlayerIndex);
//	}
//
//	CountInGamePlayers();
//}
//
//int32 ACOOP_GameMode::CountInGamePlayers()
//{
//	PlayersCount = AllPlayers.Num();
//	UE_LOG(LogTemp, Warning, TEXT("Current logged player count is: %i"), PlayersCount);
//	if (PlayersCount >= 3)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Max Players count reached: %i"), PlayersCount);
//	}
//
//	return PlayersCount;
//}