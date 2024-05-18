// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DroneProjectile.generated.h"

UCLASS()
class SPM_API ADroneProjectile : public AActor
{
	GENERATED_BODY()

public:
	ADroneProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UProjectileMovementComponent* ProjectileMovement;

public:	
	UFUNCTION(BlueprintCallable)
	void OnCollision(const FHitResult& Result);
	void SetDamage(float);
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UNiagaraSystem* HitParticle;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UDamageType> DamageType;

private:
	float Damage = 0;
};
