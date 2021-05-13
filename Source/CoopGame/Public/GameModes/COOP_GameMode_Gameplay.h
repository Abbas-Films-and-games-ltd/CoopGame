// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "COOP_GameMode_Base.h"
#include "COOP_GameMode_Gameplay.generated.h"

/**
 * 
 */

enum class EWaveState :uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

UCLASS()
class COOPGAME_API ACOOP_GameMode_Gameplay : public ACOOP_GameMode_Base
{
	GENERATED_BODY()

public:

	ACOOP_GameMode_Gameplay();

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "COOP GameMode")
		void SpawnNewBot();

	//Spawning bots
	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_NextWave;

	UPROPERTY()
		int32 NrOfBotsToSpawn;
	UPROPERTY()
		int32 WaveCount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "COOP GameMode")
		float TimerBetweenWaves;
	UPROPERTY(EditDefaultsOnly, Category = "COOP GameMode")
		int32 NrBotsMultiplier = 5;

	UFUNCTION()
		void StartWave();

	UFUNCTION()
		void SpawnBotTimerElapsed();

	//Stop spawning bots
	UFUNCTION()
		void EndWave();

	//Set Timer for next wave
	UFUNCTION()
		void PrepareNextWave();

	UFUNCTION()
		void GameOver();

	void SetWaveState(EWaveState NewState);

	void RespawnDeadPlayers();

	EWaveState CurrentWaveState;

public:

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;


	UPROPERTY(BlueprintAssignable, Category = "COOP GameMode")
		FOnActorKilled OnActorKilled;

	UFUNCTION()
		void CheckWaveState();

	UFUNCTION()
		void CheckAnyPlayerAlive();

};
