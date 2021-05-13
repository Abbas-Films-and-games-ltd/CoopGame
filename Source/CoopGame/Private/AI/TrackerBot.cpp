// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TrackerBot.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"
#include "Components/FPSHealthComponent.h"
#include "CoopGame/CoopGameCharacter.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "EngineUtils.h"

// Sets default values
ATrackerBot::ATrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComp;
	MeshComp->SetCanEverAffectNavigation(true);
	MeshComp->SetSimulatePhysics(true);

	HealthComponent = CreateDefaultSubobject<UFPSHealthComponent>(TEXT("Health Componenet"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &ATrackerBot::HandleTakeDamage);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(DestroyDamageRadius);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void ATrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		NextPathPoint = GetNextPathPoint();
	}
	
	PlayAssignedSound(BotMovingSound);
}

void ATrackerBot::HandleTakeDamage(UFPSHealthComponent* HealthComp, float Health, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("Bot Health is %s"), *FString::SanitizeFloat(Health));
	if (DamageMatInst == nullptr)
	{
		DamageMatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}
	if (DamageMatInst)
	{
		DamageMatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}

	PlayAssignedSound(BotDamageSound);

	if (Health <= 0.0f)
	{
		SelfDestruct();
	}
}

FVector ATrackerBot::GetNextPathPoint()
{
	AActor* BestTarget = nullptr;
	float NearestTargetDistance = FLT_MAX;
	TArray<AActor*> CharactersInGame;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoopGameCharacter::StaticClass(), CharactersInGame );

	if (CharactersInGame.Num() > 0)
	{
		for (int32 i = 0; i < CharactersInGame.Num(); i++)
		{
			UFPSHealthComponent* TestHComp = Cast<UFPSHealthComponent>(CharactersInGame[i]->GetComponentByClass(UFPSHealthComponent::StaticClass()));
			if (!TestHComp || TestHComp->GetHealth() <= 0)
			{
				continue;
			}
			const float TargetDistance = (GetActorLocation() - CharactersInGame[i]->GetActorLocation()).Size();

			if (TargetDistance < NearestTargetDistance)
			{
				BestTarget = CharactersInGame[i];
				NearestTargetDistance = TargetDistance;
				//UE_LOG(LogTemp, Warning, TEXT("Distance To Target: %f"), NearestTargetDistance);
			}
			
		}
		
		UNavigationPath* NavPathPoint = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), BestTarget);//GetWorld()->GetFirstPlayerController()->GetPawn());

		GetWorldTimerManager().ClearTimer(TimerHandle_RefreshPath);
		GetWorldTimerManager().SetTimer(TimerHandle_RefreshPath, this, &ATrackerBot::RefreshPath, 5.0f, false);

		if (NavPathPoint && NavPathPoint->PathPoints.Num() > 0)
		{
			return NavPathPoint->PathPoints[1];
		}
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO PAWN FOUND"));
	}
	
	return GetActorLocation();
}

void ATrackerBot::SelfDestruct()
{
	if (bExploded)
	{
		return;
	}

	bExploded = true;
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetActorLocation());
	PlayAssignedSound(BotExplodeSound);
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetVisibility(false, true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (HasAuthority())
	{
		TArray<AActor*>IgnoredActors;
		IgnoredActors.Add(this);
		UGameplayStatics::ApplyRadialDamage(this,
			DestroyDamage,
			GetActorLocation(),
			DestroyDamageRadius,
			nullptr,
			IgnoredActors,
			this,
			GetInstigatorController(),
			true);

		SetLifeSpan(2.0f);
	}
}

void ATrackerBot::RefreshPath()
{
	if (HasAuthority())
	{
		NextPathPoint = GetNextPathPoint();
	}
	
}

void ATrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
	PlayAssignedSound(BotSelfDestructSound);
}

void ATrackerBot::PlayAssignedSound(USoundBase* SoundToPlay)
{
	if (SoundToPlay)
	{
		UGameplayStatics::SpawnSoundAttached(SoundToPlay, RootComponent);
	}
}

// Called every frame
void ATrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority() && !bExploded)
	{
		float CurrentDistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

		if (CurrentDistanceToTarget <= DistanceToTarget)
		{
			//Target point reached
			NextPathPoint = GetNextPathPoint();
		}
		else
		{
			//Go to next target point
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();
			ForceDirection = ForceDirection * MovementForce;
			MeshComp->AddForce(ForceDirection, NAME_None, true);
		}
	}
}

void ATrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AController* PC = OtherActor->GetInstigatorController();
	
	if (PC != nullptr && !HealthComponent->IsFriendly(OtherActor, this))
	{
		if (!bSelfDestructionStarted && !bExploded)
		{
			ACoopGameCharacter* PlayerPawn = Cast<ACoopGameCharacter>(OtherActor);
			
			if (PlayerPawn)
			{
				if (HasAuthority())
				{
					GetWorldTimerManager().SetTimer(DestryTimerHandle, this, &ATrackerBot::DamageSelf, 1.0f, true, 0);
				}
				bSelfDestructionStarted = true;
			}
		}
	}
}
