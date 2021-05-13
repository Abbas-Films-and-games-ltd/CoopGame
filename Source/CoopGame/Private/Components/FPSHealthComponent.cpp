// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FPSHealthComponent.h"
#include "GameModes/COOP_GameMode_Gameplay.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UFPSHealthComponent::UFPSHealthComponent()
{
	SetIsReplicatedByDefault(true);
	bIsDead = false;
}


// Called when the game starts
void UFPSHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UFPSHealthComponent::TakingDamage);
		}
	}
	
	Health = DefaultHealth;
}

void UFPSHealthComponent::OnRep_Health(float OldHealth)
{
	float Damage = Health - OldHealth;
	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

void UFPSHealthComponent::TakingDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage == 0 || bIsDead)
	{
		return;
	}

	if (DamageCauser!=DamagedActor && IsFriendly(DamagedActor, DamageCauser))
	{
		return;
	}
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead)
	{
		ACOOP_GameMode_Gameplay* GM = Cast<ACOOP_GameMode_Gameplay>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}
}

void UFPSHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0 || Health <= 0 || Health >= DefaultHealth)
	{
		return;
	}
	Health = FMath::Clamp(Health + HealAmount, 0.0f, DefaultHealth);
	OnHealthChanged.Broadcast(this, Health, -HealAmount, nullptr, nullptr, nullptr);
	//UE_LOG(LogTemp, Log, TEXT("Health: %f"), Health);
}

float UFPSHealthComponent::GetHealth() const
{
	return Health;
}

bool UFPSHealthComponent::IsFriendly(AActor* ActorA, AActor* ActorB)
{
	if (ActorA == nullptr || ActorB == nullptr)
	{
		return false;
	}
	UFPSHealthComponent* ActorAHComp = Cast <UFPSHealthComponent>(ActorA->GetComponentByClass(UFPSHealthComponent::StaticClass()));
	UFPSHealthComponent* ActorBHComp = Cast <UFPSHealthComponent>(ActorB->GetComponentByClass(UFPSHealthComponent::StaticClass()));
	if (ActorAHComp == nullptr || ActorBHComp == nullptr)
	{
		return false;
	}
	return ActorAHComp->TeamNumber == ActorBHComp->TeamNumber;
}

void UFPSHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFPSHealthComponent, Health);
}
