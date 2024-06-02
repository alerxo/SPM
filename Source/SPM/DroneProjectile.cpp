// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "SPMCharacter.h"
#include "Kismet/GameplayStatics.h"

ADroneProjectile::ADroneProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	// Sets life span to make sure the projectile is destroyed
	InitialLifeSpan = 5;
}

void ADroneProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Subscribes collision method to the event
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &ADroneProjectile::OnCollision);
}

void ADroneProjectile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADroneProjectile::OnCollision(const FHitResult& Result)
{
	if (Result.GetActor())
	{
		// If we hit an actor, we apply damage to it
		UGameplayStatics::ApplyDamage(Result.GetActor(), Damage, GetInstigatorController(), this, DamageType);

		if (!Cast<ASPMCharacter>(Result.GetActor()))
		{
			// If this actor was not the player, we play the hit indicator 
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticle, Result.Location, Result.Normal.Rotation());
		}
	}

	Destroy();
}

void ADroneProjectile::SetDamage(const float Value)
{
	Damage = Value;
}
