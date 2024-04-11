// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneProjectile.h"

// Sets default values
ADroneProjectile::ADroneProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void ADroneProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADroneProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

