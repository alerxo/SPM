// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneProjectile.h"

ADroneProjectile::ADroneProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void ADroneProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ADroneProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

