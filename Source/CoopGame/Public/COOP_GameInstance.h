// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "OnlineSessionSettings.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"

#include "COOP_GameInstance.generated.h"

UCLASS()
class COOPGAME_API UCOOP_GameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UCOOP_GameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "COOP Game Instance")
	void HostGame();
	UFUNCTION(BlueprintCallable, Category = "COOP Game Instance")
	void OnCreateNewSessionComplete(FName SessionName, bool Success);
	UFUNCTION(BlueprintCallable, Category = "COOP Game Instance")
	void FindNewSessions();
	UFUNCTION(BlueprintCallable, Category = "COOP Game Instance")
	void OnFindSessionsComplete(bool Success);
	
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION(BlueprintCallable, Category = "COOP Game Instance")
	void OnDestroySessionsComplete();
	UFUNCTION(BlueprintCallable, Category = "COOP Game Instance")
	void JoinGame(int32 ServerIndex);
	UFUNCTION(BlueprintCallable, Category= "COOP Game Instance")
	void LoadMenu();
	UFUNCTION(BlueprintCallable, Category = "COOP Game Instance")
	void LoadPauseMenu();
	UFUNCTION(BlueprintCallable, Category = "COOP Game Instance")
	void ClientTravelToLevel(const FString& Address);
	UFUNCTION(BlueprintCallable, Category = "COOP Game Instance")
	void OnSessionLost();
	UPROPERTY(EditAnywhere, Category= "COOP Game Instance")
	FString MainMenuLevel = "MainMenu";

private:

	FOnlineSessionSettings SessionSettings;
	
	TSubclassOf<class UUserWidget> MainMenuClass;
	TSubclassOf<class UUserWidget> PauseMenuClass;

	class UCOOP_MainMenu* MainMenu;
	class UCOOP_PauseMenu* PauseMenu;
	
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	
};
