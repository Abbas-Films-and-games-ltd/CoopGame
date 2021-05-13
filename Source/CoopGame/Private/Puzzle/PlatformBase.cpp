// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzle/PlatformBase.h"
#include "Net/UnrealNetwork.h"

APlatformBase::APlatformBase()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
	
	bReplicates = true;	
	SetReplicatingMovement(true);
}

void APlatformBase::BeginPlay()
{
	Super::BeginPlay();

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void APlatformBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if(ActiveTriggers > 0)
		{
			FVector Location = GetActorLocation();
			
			float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
			float JourneyTravelled = (Location - GlobalStartLocation).Size();
			
			if (JourneyTravelled >= JourneyLength)
			{
				FVector SwapStartLocation = GlobalStartLocation;
				GlobalStartLocation = GlobalTargetLocation;
				GlobalTargetLocation = SwapStartLocation;
			}
			FVector	Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
			Location += Speed * DeltaTime * Direction;
			SetActorLocation((Location));
		}
		
	}
}

void APlatformBase::AddActiveTrigger()
{
	ActiveTriggers++;
}

void APlatformBase::RemoveActiveTrigger()
{
	if (ActiveTriggers > 0)
	{
		ActiveTriggers--;
	}
}
