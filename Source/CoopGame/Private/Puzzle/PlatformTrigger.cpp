// Fill out your copyright notice in the Description page of Project Settings.
#include "Puzzle/PlatformTrigger.h"
#include "Puzzle/PlatformBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	if (!ensure(TriggerVolume != nullptr)) return;
	TriggerVolume->SetBoxExtent(FVector (50.f, 50.f, 100.f));
	TriggerVolume->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	RootComponent = TriggerVolume;
	
	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("TriggerMesh"));
	TriggerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.f));
	TriggerMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.2f));
	TriggerMesh->SetupAttachment(RootComponent);
	
	TriggerLight = CreateDefaultSubobject<USpotLightComponent>(FName("TriggerLight"));
	TriggerLight->SetRelativeLocation(FVector(0.0f, 0.0f, 100.f));
	TriggerLight->SetRelativeRotation(FRotator(0.0f, -90.f, 0.0f));
	TriggerLight->SetIntensity(100000.0f);
	TriggerLight->SetLightColor(FLinearColor(0.0f, 0.5f, 0.0f, 1.f));
	TriggerLight->SetAttenuationRadius(200.f);
	TriggerLight->SetInnerConeAngle(15.f);
	TriggerLight->SetOuterConeAngle(20.f);
	TriggerLight->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapBegins);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapEnds);
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlatformTrigger::OnOverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("OVERLAP BEGINS"));
	for (APlatformBase* Platform : PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
	}
}

void APlatformTrigger::OnOverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("OVERLAP ENDS"));
	for (APlatformBase* Platform : PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}
}
