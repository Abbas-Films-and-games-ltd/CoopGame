// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/COOP_PauseMenu.h"
#include "Components/Button.h"

bool UCOOP_PauseMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }
	if (Resume_BTN == nullptr) { return false; }
	Resume_BTN->OnClicked.AddDynamic(this, &UCOOP_PauseMenu::ResumeGame);
	if (BackToMainMenu_BTN == nullptr) { return false; }
	BackToMainMenu_BTN->OnClicked.AddDynamic(this, &UCOOP_PauseMenu::BackToMainMenu);

	return true;
}

void UCOOP_PauseMenu::ResumeGame()
{
	Teardown();
	UE_LOG(LogTemp, Warning, TEXT("RESUME!"));
}

void UCOOP_PauseMenu::BackToMainMenu()
{
	const FString& LevelToTravelTo = FString(TEXT("/Game/Maps/MainMenu"));
	if (MenuInterface != nullptr)
	{
		if (LevelToTravelTo.Len() > 0)
		{
			Teardown();
			MenuInterface->ClientTravelToLevel(LevelToTravelTo);
			UE_LOG(LogTemp, Warning, TEXT("BACK TO MAIN MENU!"));
		}
	}
}
