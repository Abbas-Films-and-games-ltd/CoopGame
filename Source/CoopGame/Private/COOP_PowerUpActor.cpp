// Fill out your copyright notice in the Description page of Project Settings.


#include "COOP_PowerUpActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACOOP_PowerUpActor::ACOOP_PowerUpActor()
{
	bReplicates = true;
	SetReplicatingMovement(true);
	
	bIsPowerupActive = false;
}

void ACOOP_PowerUpActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACOOP_PowerUpActor::OnTickPowerup()
{
	TicksProcessed++;
	
	OnPowerupTicked();

	if (TicksProcessed >= TotalTicks)
	{
		OnExpiring();

		bIsPowerupActive = false;
		OnRep_PoweruoActivate();

		GetWorldTimerManager().ClearTimer(PowerupTickHandle);
	}
}

void ACOOP_PowerUpActor::OnRep_PoweruoActivate()
{
	OnPowerupStateChange(bIsPowerupActive);
}

void ACOOP_PowerUpActor::ActivatePowerup(AActor* PickingUpActor)
{
	OnActivating(PickingUpActor);

	bIsPowerupActive = true;
	OnRep_PoweruoActivate();

	if (PowerupInterval > 0)
	{
		GetWorldTimerManager().SetTimer(PowerupTickHandle, this, &ACOOP_PowerUpActor::OnTickPowerup, PowerupInterval, true);
	}
	else
	{
		OnTickPowerup();
	}
}

void ACOOP_PowerUpActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACOOP_PowerUpActor, bIsPowerupActive);
}