// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyInterface.h"
#include "FlyingMovementComponent.h"
#include "GameFramework/Character.h"
#include "Drone.generated.h"

UCLASS()
class SPM_API ADrone : public APawn, public IEnemyInterface
{
	GENERATED_BODY()

public:
	ADrone();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	UFUNCTION(BlueprintCallable)
	void AimAtPosition(const FVector Position) const;
	UFUNCTION(BlueprintCallable)
	void SetAimPitch(const float Pitch) const;
	UFUNCTION(BlueprintCallable)
	void Shoot(const bool UseNormalDamage);
	UFUNCTION(BlueprintCallable)
	void Reload();
	UFUNCTION(BlueprintCallable)
	void CheckLineOfSightAtPlayer();

	UFUNCTION(BlueprintCallable)
	FVector GetKiteLocation() const;
	UFUNCTION(BlueprintCallable)
	FVector GetPatrolLocation() const;
	UFUNCTION(BlueprintCallable)
	FVector GetStrafeLocation(const int State) const;
	UFUNCTION(BlueprintCallable)
	bool HasTarget() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnShoot(bool IsLeftFire);

public:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* WeaponBaseLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* WeaponLookAtLeft;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* WeaponBaseRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* WeaponLookAtRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFlyingMovementComponent* FlyingMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ADroneProjectile> Projectile;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int Ammo = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float AttackSpeed = 0.15f;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float ReloadSpeed = 1.2f;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int AttackRange = 1500;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int KiteRange = 150;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int ChaseRange = 4000;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float DistanceToTarget = 0.0f;
	
	UPROPERTY(BlueprintReadWrite)
	bool CanKite = true;
	UPROPERTY(BlueprintReadWrite)
	bool CanStrafe = true;
	UPROPERTY(BlueprintReadWrite)
	bool IsStrafing = false;

	UPROPERTY(EditAnywhere)
	bool Debug = false;

private:
	UPROPERTY()
	AActor* Player;
	UPROPERTY()
	AActor* Target;
	
	bool LeftFire = false;
	
	const float PatrolPitch = 10.0f;
	const float PatrolYaw = 180.0f;
	const int PatrolMin = 500;
	const int PatrolMax = 1000;
	
	float Health = 0.0f;
	const float MaxHealth = 14.0f;
	const float Damage = 2.0f;
	const float StrafeDamage = 3.0f;
	const int MaxAmmo = 4;
	const float AccuracyMargin = 3.0f;
	const int AimPitch = 25;
	const float AimYawCorrection = 3.0f;
	const int KiteYaw = 40;
	const int StrafeDistance = 1800;
	const int StrafePitch = 12;
	const int StrafeOffset = 400;
};