// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"

#include "DroneProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"

// Sets default values
ADrone::ADrone()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ADrone::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float TakenDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	TakenDamage = FMath::Min(Health, TakenDamage);
	Health -= TakenDamage;
	
	UE_LOG(LogTemp, Log, TEXT("Drone Damage Taken %F, Health Left %F"), TakenDamage, Health);

	if(Health <= 0)
	{
		Destroy();
	}
	
	return TakenDamage;
}


void ADrone::ShootTarget()
{
	AActor* player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if(player)
	{
		FVector Origin = GetOwner()->GetActorLocation();
		FVector Target = player->GetActorLocation();
		FVector Rotation = Target-Origin.Normalize();
		GetWorld()->SpawnActor<ADroneProjectile>(Projectile, Origin, Rotation.Rotation());
	}
}
