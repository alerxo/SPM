// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UProjectileMovementComponent* ProjectileMovement;

public:	
	virtual void Tick(float DeltaTime) override;
};
