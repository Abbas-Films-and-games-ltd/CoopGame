// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformTrigger.generated.h"

UCLASS()
class COOPGAME_API APlatformTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlatformTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	
private:
	UPROPERTY(VisibleAnywhere, Category=Components)
	class UBoxComponent* TriggerVolume;
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UStaticMeshComponent* TriggerMesh;
	UPROPERTY(VisibleAnywhere, Category = Components)
	class USpotLightComponent* TriggerLight;

	UFUNCTION()
	void OnOverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category=PuzzleSettings)
	TArray<class APlatformBase*>PlatformsToTrigger;
};
