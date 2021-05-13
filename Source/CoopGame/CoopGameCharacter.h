// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CoopGameCharacter.generated.h"

class ASWeapon;
class UFPSHealthComponent;

UCLASS(config=Game)
class ACoopGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ACoopGameCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Camera)
	FRotator ControllerRotation;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void BeginCrouch();
	void EndCrouch();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFPSHealthComponent* HealthComponent;

	UPROPERTY(Replicated)
	ASWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ASWeapon> NewWeapon;

	UPROPERTY(VisibleDefaultsOnly)
	FName WeaponSocket = "WeaponSocket";
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV = 65.f;
	bool bIsZooming;
	float DefaultFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomInterpSpeed = 50;

	void BeginZoom();
	void EndZoom();
	
	UFUNCTION()
	void OnHealthChanged(UFPSHealthComponent* HealthComp, float Health, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAiming(bool bZooming);
	
public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual FVector GetPawnViewLocation() const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void ZoomBegin();
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void ZoomEnd();

	UFUNCTION(BlueprintCallable)
	void EquipingWeapon(/*TSubclassOf<ASWeapon> NewWeapon*/);
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bIsDead;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Player")
	uint8 KilledScore = 20;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	bool bAiming;

	UFUNCTION(BlueprintCallable, Category= "Player")
	void StartFire();
	UFUNCTION(BlueprintCallable, Category= "Player")
	void StopFire();

};