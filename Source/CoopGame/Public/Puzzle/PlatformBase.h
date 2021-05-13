// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "PlatformBase.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API APlatformBase : public AStaticMeshActor
{
	GENERATED_BODY()

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	APlatformBase();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category= PuzzleSettings, meta=(MakeEditWidget=true))
	FVector TargetLocation = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, Category = PuzzleSettings)
	float Speed = 100;

	void AddActiveTrigger();
	void RemoveActiveTrigger();
	
private:
	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;

	UPROPERTY(EditAnywhere, Category = PuzzleSettings)
	int32 ActiveTriggers = 0;
};
