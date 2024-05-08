// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyInterface.h"
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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void Aim(const FVector Position) const;
	UFUNCTION(BlueprintCallable)
	void Shoot();
	UFUNCTION(BlueprintCallable)
	void Reload();
	UFUNCTION(BlueprintCallable)
	void MoveTo(const FVector Position);
	UFUNCTION(BlueprintCallable)
	FVector GetKiteLocation() const;
	UFUNCTION(BlueprintCallable)
	FVector GetPatrolLocation()const;
	UFUNCTION(BlueprintCallable)
	bool HasTarget() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnShoot(bool IsLeftFire);

private:
	void CheckLineOfSightAtPlayer();
	void Rotate();
	void GetTargetVelocity();
	void CheckLidarDirection(FRotator);
	void Move(const float);

public:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* WeaponBaseLeft;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* WeaponLookAtLeft;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* WeaponBaseRight;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* WeaponLookAtRight;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ADroneProjectile> Projectile;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int Ammo = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int AttackRange = 1500;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int KiteRange = 250;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int ChaseRange = 3500;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float DistanceToTarget = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool HasDestination = false;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector Velocity;
	
	UPROPERTY(EditAnywhere)
	bool Debug = false;

private:
	UPROPERTY()
	AActor* Player;
	UPROPERTY()
	AActor* Target;
	bool IsInCombat = false;
	bool LeftFire = false;
	int TickCount = 0;
	const int TickInterval = 20;

	FRotator MovementDirection;
	FVector Destination;
	FVector TargetVelocity;
	FRotator TargetRotation;
	const int MovementSpeed = 700;
	const float Acceleration = 0.8f;
	const int StopDistance = 100;
	const int ObstacleAvoidanceDistance = 200;
	const float ObstacleAvoidanceForce = 2.0f;
	const TArray<FRotator> LidarDirections =
	{
		FRotator(0, 0,0),
		FRotator(30, 30,0),
		FRotator(30, -30,0),
		FRotator(-30, 30,0),
		FRotator(-30, -30,0),
		FRotator(90, 0,0),
		FRotator(-90, 0,0)
	};

	float Health = 0.0f;
	const float MaxHealth = 10.0f;
	const float Damage = 0.5f;
	const int MaxAmmo = 4;
	const float AttackSpeed = 0.1f;
	const float ReloadSpeed = 1.0f;
	const float AccuracyMargin = 3.0f;
	const int AimPitch = 25;
	const float AimYawCorrection = 3.0f;
	const int KiteYawDegree = 40;
};