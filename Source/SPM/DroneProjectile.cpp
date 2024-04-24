// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneProjectile.h"

#include "Kismet/GameplayStatics.h"

ADroneProjectile::ADroneProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	InitialLifeSpan = 5;
}

void ADroneProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement->OnProjectileStop.AddDynamic(this, &ADroneProjectile::OnCollision);
}

void ADroneProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADroneProjectile::OnCollision(const FHitResult& Result)
{
	if(Result.GetActor())
	{
		UGameplayStatics::ApplyDamage(Result.GetActor(), Damage, GetInstigatorController(), this, nullptr);
	}

	Destroy();
}

void ADroneProjectile::SetDamage(const float Value)
{
	Damage = Value;
}
