// Fill out your copyright notice in the Description page of Project Settings.


#include "COOP_PickupBase.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "COOP_PowerUpActor.h"
#include "CoopGame/CoopGameCharacter.h"

// Sets default values
ACOOP_PickupBase::ACOOP_PickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75);
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90, 0, 0));
	DecalComp->DecalSize = FVector(65,75,75);
	DecalComp->SetupAttachment(RootComponent);

	bReplicates = true;
}

// Called when the game starts or when spawned
void ACOOP_PickupBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Respawn();
	}
}

void ACOOP_PickupBase::Respawn()
{
	if (PowerUpClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Powerup Class is missing!! Please update %s"), *GetName());
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PowerUpInstance = GetWorld()->SpawnActor<ACOOP_PowerUpActor>(PowerUpClass, GetTransform(), SpawnParams);
	//UE_LOG(LogTemp, Warning, TEXT("Powerup spawned"));
}

void ACOOP_PickupBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (HasAuthority())
	{
		if (PowerUpInstance)
		{
			PowerUpInstance->ActivatePowerup(OtherActor);
			PowerUpInstance = nullptr;

			GetWorldTimerManager().SetTimer(RespawnHandle, this, &ACOOP_PickupBase::Respawn, CoolDownDuration, false);
		}
	}
}
