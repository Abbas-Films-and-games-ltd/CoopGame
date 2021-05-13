// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include <Runtime\Engine\Public\Net\UnrealNetwork.h>
#include "Math/UnrealMathUtility.h"
#include "Components/FPSHealthComponent.h"
#include "CoopGame/CoopGameCharacter.h"

// Sets default values
ASWeapon::ASWeapon()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MuzzleSocketName = "MuzzleFlashSocket";
	TracerTargetName = "BeamEnd";

	bReplicates = true;
}

void ASWeapon::StartFire()
{
	if (!bIsShooting)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ASWeapon::Fire, FireRate, true, 0.0f);
		bIsShooting = true;
	}
	
}

void ASWeapon::StopFire()
{
	if (bIsShooting)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		bIsShooting = false;
	}
}

void ASWeapon::Fire()
{
	if (!HasAuthority())
	{
		ServerFire();
	}
	AActor*  MyOwner = GetOwner();
	if (MyOwner)
	{
		UFPSHealthComponent* OwnerHComp = Cast<UFPSHealthComponent>(MyOwner->GetComponentByClass(UFPSHealthComponent::StaticClass()));
		if (OwnerHComp->GetHealth() > 0)
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
			
			bool bIsAiming = Cast<ACoopGameCharacter>(MyOwner)->bAiming;
			float UsedWeaponSpread;
			if (bIsAiming)
			{
				UsedWeaponSpread = FMath::Clamp((WeaponSpread / 2), 1.0f, WeaponSpread) ;
			}
			else
			{
				UsedWeaponSpread = WeaponSpread;
			}
			
			float RandWeaponSpread = FMath::RandRange(UsedWeaponSpread * -1, UsedWeaponSpread);

			FVector ShotDirection = EyeRotation.Vector();

			float HalfRad = FMath::DegreesToRadians(UsedWeaponSpread);
			ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

			FVector TraceEnd = EyeLocation + (ShotDirection * 10000) + RandWeaponSpread;
			FVector TracerEndPoint = TraceEnd;

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(MyOwner);
			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;

			FHitResult Hit;
			if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
			{
				AActor* HitActor = Hit.GetActor();
				
				UGameplayStatics::ApplyPointDamage(HitActor, WeaponDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), MyOwner, DamageType);

				PlayImpactFX(Hit.ImpactPoint + RandWeaponSpread);
				TracerEndPoint = Hit.ImpactPoint;
			}

			PlayFireFX(TracerEndPoint + RandWeaponSpread);

			if (HasAuthority())
			{
				HitScanTrace.TraceTo = TracerEndPoint + RandWeaponSpread;
			}
			
		}
		
		
	}
	
}

void ASWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASWeapon::ServerFire_Validate()
{
	return true;
}

void ASWeapon::PlayFireFX(FVector TracerEndPoint)
{
	if (MuzzleFX)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFX, MeshComponent, MuzzleSocketName);
	}

	if (TracerFX)
	{
		FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TracerFXComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerFX, MuzzleLocation);
		if (TracerFXComp)
		{
			TracerFXComp->SetVectorParameter(TracerTargetName, TracerEndPoint);
		}

	}

	APawn* MyOwner = Cast<APawn>(GetOwner());
	APlayerController* PlayerPC = Cast<APlayerController>(MyOwner->GetInstigatorController());
	if (PlayerPC)
	{
		PlayerPC->ClientStartCameraShake(CamShake);
	}
}

void ASWeapon::PlayImpactFX(FVector ImpactPoint)
{
	if (ImpactFX)
			{
				FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
				FVector ShotDirection = ImpactPoint - MuzzleLocation;
				ShotDirection.Normalize();
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, ImpactPoint, ShotDirection.Rotation());
			}
}

void ASWeapon::OnRep_HitScanTrace()
{
	PlayFireFX(HitScanTrace.TraceTo);
	PlayImpactFX(HitScanTrace.TraceTo);
}

void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASWeapon, HitScanTrace, COND_SkipOwner);
}
