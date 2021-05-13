// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/MenuWidget.h"
#include "COOP_MainMenu.generated.h"

UCLASS()
class COOPGAME_API UCOOP_MainMenu : public UMenuWidget
{
	GENERATED_BODY()
public:
	UCOOP_MainMenu(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual  bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Host_BTN;
	UPROPERTY(meta = (BindWidget))
	class UButton* Join_Menu_BTN;
	UPROPERTY(meta = (BindWidget))
	class UButton* Refresh_BTN;
	UPROPERTY(meta = (BindWidget))
	class UButton* Back_BTN;
	UPROPERTY(meta = (BindWidget))
	class UButton* Exit_BTN;
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitch;
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu_VBox;
	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu_VBox;
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerListSBox;

	TSubclassOf<class UUserWidget> ServerListEntity;

	UFUNCTION()
	void ExitGame();
	
	UFUNCTION()
	void HostGame();

	UFUNCTION()
	void RefreshServerList();

	UFUNCTION()
	void OpenJoinMenu();
	UFUNCTION()
	void OpenMainMenu();

public:
	UFUNCTION()
	void AddEntityToServerList(TArray<FString> ServerNames, int32 NumAvaiableSlots, int32 MaxPlayerAllowed, FString ServerOwnerName);
	TArray<FString> ServersListNames;
	UFUNCTION()
	void JoinGame(int32 ServerIndex);

	/*void SelectIndex(int32 Index);
	TOptional<int32> SelectedIndex;*/
};