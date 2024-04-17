// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Drone.generated.h"

UCLASS()
class SPM_API ADrone : public ACharacter
{
	GENERATED_BODY()

public:
	ADrone();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void ShootTarget();

public:
	
	UPROPERTY(VisibleDefaultsOnly)
	USkeletalMeshComponent* StableMesh;
	
	UPROPERTY(VisibleDefaultsOnly)
	class UPhysicsConstraintComponent* PhysicsConstraint;
	
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* ConstraintMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ADroneProjectile> Projectile;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float Health = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USceneComponent* ProjectileOrigin;
};
