// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Drone.generated.h"

UCLASS()
class SPM_API ADrone : public APawn
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
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void Aim(const FVector Position) const;
	UFUNCTION(BlueprintCallable)
	void Shoot();
	UFUNCTION(BlueprintCallable)
	void Reload();
	UFUNCTION(BlueprintCallable)
	void MoveTo(const FVector Position);
	UFUNCTION(BlueprintCallable)
	void SetFocus(AActor* Target);
	UFUNCTION(BlueprintCallable)
	void ClearFocus();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void OnShoot(bool IsLeftFire);

private:
	void CheckLineOfSightAtPlayer() const;
	void Rotate();
	void GetTargetVelocity();
	void CheckLidarDirection(FRotator);
	void GetGravity();
	void Move(const float);

public:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Root;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* WeaponBaseLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* ProjectileOriginLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* WeaponBaseRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* ProjectileOriginRight;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ADroneProjectile> Projectile;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float Health = 0;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combat")
	float Damage = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float Ammo = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int AmmoCount = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackSpeed = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ReloadSpeed = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackRange = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AccuracyMargin = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MovementSpeed = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float Acceleration = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float StopDistance = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float ObstacleAvoidanceDistance = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float ObstacleAvoidanceForce = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float Gravity = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool HasDestination;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FRotator MovementDirection;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector Destination;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector Velocity;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector TargetVelocity;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FRotator TargetRotation;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	TArray<FRotator> LidarDirections = {};

	UPROPERTY(EditAnywhere)
	bool Debug = false;

private:
	UPROPERTY()
	AActor* Player;
	UPROPERTY()
	class UBlackboardComponent* BlackboardComponent;
	UPROPERTY()
	AActor* Focus;
	bool LeftFire = false;
};
