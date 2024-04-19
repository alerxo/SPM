// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"

#include "DroneProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

ADrone::ADrone()
{
	PrimaryActorTick.bCanEverTick = true;

	StableMesh = FindComponentByClass<USkeletalMeshComponent>();
	
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(StableMesh);

	ConstraintMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConstraintMesh"));
	ConstraintMesh->SetupAttachment(PhysicsConstraint);

	ProjectileOriginLeft = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileOriginLeft"));
	ProjectileOriginLeft->SetupAttachment(ConstraintMesh);

	ProjectileOriginRight = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileOriginRight"));
	ProjectileOriginRight->SetupAttachment(ConstraintMesh);
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();
	
	PhysicsConstraint->SetConstrainedComponents(StableMesh, NAME_None, ConstraintMesh, NAME_None);
}

void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ADrone::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float const TakenDamage = FMath::Min(Health, Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser));;
	
	if((Health -= TakenDamage) <= 0)
	{
		GetController()->Destroy();
		Destroy();
	}
	
	return TakenDamage;
}

void ADrone::ShootTarget()
{
	if(const AActor* Player = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		LeftFire = !LeftFire;
		
		FVector Origin = LeftFire ? ProjectileOriginLeft->GetComponentLocation() : ProjectileOriginRight->GetComponentLocation();
		FRotator Rotation = LeftFire ? ProjectileOriginLeft->GetComponentRotation() : ProjectileOriginRight->GetComponentRotation();

		FVector Position = Origin;
		
		
		ADroneProjectile* NewProjectile = GetWorld()->SpawnActor<ADroneProjectile>(Projectile, Origin, Rotation);
		NewProjectile->SetOwner(this);
	}
}
