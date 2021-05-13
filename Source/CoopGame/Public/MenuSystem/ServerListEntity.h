// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "COOP_MainMenu.h"
#include "ServerListEntity.generated.h"

UCLASS()
class COOPGAME_API UServerListEntity : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual  bool Initialize() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Join_BTN;
	
	UFUNCTION()
	void JoinGame();
	UPROPERTY()
	UCOOP_MainMenu* MainMenu;
	UPROPERTY()
	int32 ServerIndex = 0;
	
public:
	void SetupEntity(class UCOOP_MainMenu* ParentMenu, int32 InServerIndex, FString* InServerName, int32 NumAvaiableSlots, int32 MaxPlayerAllowed);
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName_TXT;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AvaiableSlots;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxPlayers;
	UPROPERTY(EditAnywhere)
	FString ServerName;
};
