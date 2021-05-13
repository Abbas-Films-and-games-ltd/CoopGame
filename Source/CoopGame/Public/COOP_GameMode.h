// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "OnlineSessionSettings.h"
#include "COOP_GameMode.generated.h"




//enum class EWaveState:uint8;
//
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

UCLASS()
class COOPGAME_API ACOOP_GameMode : public AGameModeBase
{
	GENERATED_BODY()
//protected:
//
//	UFUNCTION(BlueprintImplementableEvent, Category = "COOP Game Mode")
//	void SpawnNewBot();
//
//	//Spawning bots
//	FTimerHandle TimerHandle_BotSpawner;
//
//	FTimerHandle TimerHandle_NextWave;
//
//	UPROPERTY()
//	int32 NrOfBotsToSpawn;
//	UPROPERTY()
//	int32 WaveCount = 0;
//
//	UPROPERTY(EditDefaultsOnly, Category = "COOP Game Mode")
//	float TimerBetweenWaves;
//	UPROPERTY(EditDefaultsOnly, Category = "COOP Game Mode")
//	int32 NrBotsMultiplier = 5;
//
//	UFUNCTION()
//	void StartWave();
//
//	UFUNCTION()
//	void SpawnBotTimerElapsed();
//
//	//Stop spawning bots
//	UFUNCTION()
//	void EndWave();
//
//	//Set Timer for next wave
//	UFUNCTION()
//	void PrepareNextWave();
//
//	UFUNCTION()
//	void GameOver();
//
//	void SetWaveState(EWaveState NewState);
//
//	void RespawnDeadPlayers();
//
//	EWaveState CurrentWaveState;
//
//public:
//	
//	ACOOP_GameMode();
//
//	virtual void StartPlay() override;
//
//	virtual void Tick(float DeltaSeconds) override;
//
//	UPROPERTY(EditDefaultsOnly, Category="COOP Game Mode")
//	bool bIsLobby = true;
//
//	UPROPERTY(BlueprintAssignable, Category = "COOP Game Mode")
//	FOnActorKilled OnActorKilled;
//
//	UFUNCTION()
//	void CheckWaveState();
//
//	UFUNCTION()
//	void CheckAnyPlayerAlive();
//
//	virtual void PostLogin(APlayerController* NewPlayer) override;
//
//	virtual void Logout(AController* Exiting) override;
//
//	
//	UPROPERTY(EditDefaultsOnly, Category = "COOP GameMode")
//		bool bIsLANMatch = true;
//	UPROPERTY(EditDefaultsOnly, Category = "COOP GameMode")
//		int32 MaxPlayers = 4;
//	UPROPERTY(EditDefaultsOnly, Category = "COOP GameMode")
//		FString LevelName = "Blockout_P";
//	
//private:
//	
//	UPROPERTY()
//		TArray<AController*> AllPlayers;
//
//	UPROPERTY()
//		int32 PlayersCount;
//
//	UFUNCTION(BlueprintCallable, Category = "COOP GameMode")
//		int32 CountInGamePlayers();

};
