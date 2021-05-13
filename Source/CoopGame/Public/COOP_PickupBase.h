// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "COOP_PickupBase.generated.h"

class USphereComponent;
class UDecalComponent;
class ACOOP_PowerUpActor;

UCLASS()
class COOPGAME_API ACOOP_PickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACOOP_PickupBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComp;

	UPROPERTY(EditAnywhere, Category = "PickupActor")
	TSubclassOf<ACOOP_PowerUpActor> PowerUpClass;
	UPROPERTY(EditAnywhere, Category = "PickupActor")
	float CoolDownDuration;

	ACOOP_PowerUpActor* PowerUpInstance;

	FTimerHandle RespawnHandle;

	UFUNCTION()
	void Respawn();

public:

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
