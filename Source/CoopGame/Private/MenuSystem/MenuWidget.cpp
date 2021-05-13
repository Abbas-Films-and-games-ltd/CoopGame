// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* MainMenuInterface)
{
	this->MenuInterface = MainMenuInterface;
}

void UMenuWidget::Setup()
{
	this->AddToViewport(5);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr) { return; }
	FInputModeUIOnly InputModeUIData;
	PlayerController->SetInputMode(InputModeUIData);
	PlayerController->SetShowMouseCursor(true);
	InputModeUIData.SetWidgetToFocus(this->TakeWidget());
	InputModeUIData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
}

void UMenuWidget::Teardown()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr) { return; }
	FInputModeGameOnly InputModeGameData;
	PlayerController->SetInputMode(InputModeGameData);
	PlayerController->SetShowMouseCursor(false);

	UE_LOG(LogTemp, Warning, TEXT("Removing Menu!"));
	RemoveFromParent();
}