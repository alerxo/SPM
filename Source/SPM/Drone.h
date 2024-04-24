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
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void ShootTarget();

	UFUNCTION(BlueprintCallable)
	void Reload();
	
	UFUNCTION(BlueprintCallable)
	void SetDestination(const FVector Position);

private:
	void TryLookAtPlayer() const;
	void MoveTowardsDestination();
	void ObstacleAvoidance();
	void Movement(const float) const;

public:
	UPROPERTY(VisibleDefaultsOnly)
	UCapsuleComponent* Root;
	UPROPERTY(VisibleDefaultsOnly)
	USkeletalMeshComponent* StableMesh;
	UPROPERTY(VisibleDefaultsOnly)
	class UPhysicsConstraintComponent* PhysicsConstraint;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UStaticMeshComponent* ConstraintMesh;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combat")
	USkeletalMeshComponent* WeaponLeft;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combat")
	USkeletalMeshComponent* WeaponRight;
	
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
	float HoverSpeed = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float TargetHeight = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float HoverMargin = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector TargetVelocity;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float Height;
	
private:
	AActor* Player;
	class UBlackboardComponent* BlackboardComponent;
	FVector Destination;
	bool LeftFire = false;
};
