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
	void SetFocus(AActor* Target);
	UFUNCTION(BlueprintCallable)
	void ClearFocus();
	UFUNCTION(BlueprintCallable)
	void EnterCombat();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (AllowPrivateAccess = true))
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* WeaponBaseLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* WeaponLookAtLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* WeaponBaseRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* WeaponLookAtRight;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ADroneProjectile> Projectile;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float Health = 0;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combat")
	float Damage = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	int Ammo = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int AmmoCount = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackSpeed = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ReloadSpeed = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	int AttackRange = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	int KiteRange = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AccuracyMargin = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	int MovementSpeed = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float Acceleration = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	int StopDistance = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	int ObstacleAvoidanceDistance = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float ObstacleAvoidanceForce = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	TArray<FRotator> LidarDirections = {};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool HasDestination;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector Velocity;
	
	UPROPERTY(EditAnywhere)
	bool Debug = false;

private:
	UPROPERTY()
	AActor* Player;
	UPROPERTY()
	class UBlackboardComponent* BlackboardComponent;
	UPROPERTY()
	AActor* Focus;
	FRotator MovementDirection;
	FVector Destination;
	FVector TargetVelocity;
	FRotator TargetRotation;
	bool IsInCombat = false;
	bool LeftFire = false;
	int TickCount = 0;
	const int TickInterval = 10;
};