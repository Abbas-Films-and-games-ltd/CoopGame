// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "TrackerBot.generated.h"


class UFPSHealthComponent;
class USoundBase;

UCLASS()
class COOPGAME_API ATrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UFPSHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent* SphereComponent;

	UFUNCTION()
	void HandleTakeDamage(UFPSHealthComponent* HealthComp, float Health, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	FVector GetNextPathPoint();
	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Track Bot")
	float MovementForce = 1000;
	UPROPERTY(EditDefaultsOnly, Category = "Track Bot")
	float DistanceToTarget = 100;

	//Dynamic Material for taking damage
	UMaterialInstanceDynamic* DamageMatInst;

	void SelfDestruct();
	UPROPERTY(EditDefaultsOnly, Category = "Track Bot")
	UParticleSystem* ExplosionFX;
	UPROPERTY(EditDefaultsOnly, Category = "Track Bot")
	float DestroyDamage = 50;
	UPROPERTY(EditDefaultsOnly, Category = "Track Bot")
	float DestroyDamageRadius = 300;
	bool bExploded = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
	uint8 KilledScore = 10;

	FTimerHandle DestryTimerHandle;
	FTimerHandle TimerHandle_RefreshPath;

	UFUNCTION()
	void RefreshPath();
	
	UFUNCTION()
	void DamageSelf();

	UPROPERTY()
	bool bSelfDestructionStarted;

	UPROPERTY(EditDefaultsOnly, Category = "Track Bot")
	USoundBase* BotMovingSound;
	UPROPERTY(EditDefaultsOnly, Category = "Track Bot")
	USoundBase* BotDamageSound;
	UPROPERTY(EditDefaultsOnly, Category = "Track Bot")
	USoundBase* BotSelfDestructSound;
	UPROPERTY(EditDefaultsOnly, Category = "Track Bot")
	USoundBase* BotExplodeSound;

	UFUNCTION()
	void PlayAssignedSound(USoundBase* SoundToPlay);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
