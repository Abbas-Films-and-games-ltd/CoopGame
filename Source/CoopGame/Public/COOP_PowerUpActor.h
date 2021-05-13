// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "COOP_PowerUpActor.generated.h"

UCLASS()
class COOPGAME_API ACOOP_PowerUpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACOOP_PowerUpActor();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float PowerupInterval = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 TotalTicks = 5;

	FTimerHandle PowerupTickHandle;
	UFUNCTION()
	void OnTickPowerup();
	
	int32 TicksProcessed;

	UPROPERTY(ReplicatedUsing=OnRep_PoweruoActivate)
	bool bIsPowerupActive;

	UFUNCTION()
	void OnRep_PoweruoActivate();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupStateChange(bool bIsPowerupStatedChanged);

public:	
	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnActivating(AActor* PickingUpActor);
	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupTicked();
	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnExpiring();
	

	UFUNCTION()
	void ActivatePowerup(AActor* PickingUpActor);
};
