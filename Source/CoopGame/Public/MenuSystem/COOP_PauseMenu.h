// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/MenuWidget.h"
#include "COOP_PauseMenu.generated.h"

UCLASS()
class COOPGAME_API UCOOP_PauseMenu : public UMenuWidget
{
	GENERATED_BODY()
protected:
	virtual  bool Initialize() override;
	
private:
	
	UPROPERTY(meta=(BineWidget))
	class UButton* Resume_BTN;

	UPROPERTY(meta = (BineWidget))
	class UButton* BackToMainMenu_BTN;

	UFUNCTION()
	void ResumeGame();
	UFUNCTION()
	void BackToMainMenu();
};
