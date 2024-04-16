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
	// Sets default values for this character's properties
	ADrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void ShootTarget();

	UPROPERTY(EditAnywhere)
	float Health = 10;

	UPROPERTY(VisibleDefaultsOnly)
	class UPhysicsConstraintComponent* PhysicsConstraint;

	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* StableMesh;
	
	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* ConstraintMesh;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ADroneProjectile> Projectile;
};
