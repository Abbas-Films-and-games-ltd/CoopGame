// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuSystem/ServerListEntity.h"

#include <string>

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MenuSystem/COOP_MainMenu.h"
#include "Internationalization/Text.h"

class IMenuInterface;
bool UServerListEntity::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }
	if (Join_BTN == nullptr) { return false; }
	Join_BTN->OnClicked.AddDynamic(this, &UServerListEntity::JoinGame);

	return true;
}

void UServerListEntity::JoinGame()
{
	if (MainMenu == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NO MAIN MENU ASSIGNED!"));
		return;
	}
	
	ServerName = FString ("ServerName_TXT");
	if (ServerName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("NO SERVER NAME ASSIGNED!"));
		return;
	}
	
	MainMenu->JoinGame(ServerIndex);
}

void UServerListEntity::SetupEntity(UCOOP_MainMenu* ParentMenu, int32 InServerIndex, FString* InServerName, int32 NumAvaiableSlots, int32 MaxPlayerAllowed)
{
	MainMenu = ParentMenu;
	ServerIndex = InServerIndex;
	ServerName = *InServerName;
	ServerName_TXT->SetText(FText::FromString(*InServerName));
	AvaiableSlots->SetText(FText::AsNumber(NumAvaiableSlots));
	MaxPlayers->SetText(FText::AsNumber(MaxPlayerAllowed));
}
