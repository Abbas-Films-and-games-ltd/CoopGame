// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "OnlineSessionSettings.h"
#include "GameFramework/GameModeBase.h"
#include "COOP_GameMode_Base.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API ACOOP_GameMode_Base : public AGameModeBase
{
	GENERATED_BODY()
public:

	ACOOP_GameMode_Base();

	FOnlineSessionSettings GameSessionSettings;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

protected:
	
	UPROPERTY()
	TArray<AController*> AllPlayers;
	UPROPERTY()
	int32 PlayersCount;
	UPROPERTY(EditDefaultsOnly, Category = "COOP GameMode")
	bool bIsLANMatch = true;
	UPROPERTY(EditDefaultsOnly, Category = "COOP GameMode")
	int32 MaxPlayers = 2;
	UPROPERTY(EditDefaultsOnly, Category = "COOP GameMode")
	FString NextLevelName = "Lobby";
	UPROPERTY(EditDefaultsOnly, Category = "COOP Game Mode")
	bool bIsLobby = true;
	UPROPERTY(EditDefaultsOnly, Category = "COOP GameMode")
	bool bIsPublicGame = true;
	UPROPERTY(EditDefaultsOnly, Category = "COOP GameMode")
	bool bIsDedicatedServer = false;
	UPROPERTY(EditDefaultsOnly, Category = "COOP GameMode")
	bool bUseLobbySystem = true;

	bool bReady = false;
	
	UFUNCTION(BlueprintCallable, Category = "COOP GameMode")
	void GoToNextLevel();
private:

	UFUNCTION(BlueprintCallable, Category = "COOP GameMode")
	int32 CountInGamePlayers(AController* InNewPlayer);

	

};
