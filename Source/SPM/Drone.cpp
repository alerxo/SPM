// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"

#include "DroneProjectile.h"
#include "SPMCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
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

	WeaponLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponLeft"));
	WeaponLeft->SetupAttachment(ConstraintMesh);

	WeaponRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponRight"));
	WeaponRight->SetupAttachment(ConstraintMesh);
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();
	
	PhysicsConstraint->SetConstrainedComponents(StableMesh, NAME_None, ConstraintMesh, NAME_None);
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(this);

	BlackboardComponent->SetValueAsFloat("AttackSpeed", AttackSpeed);
	AmmoCount = Ammo;
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADrone::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	TryLookAtPlayer();
}

void ADrone::TryLookAtPlayer() const
{
	if(!Player) return;
	
	FHitResult Result;
	FVector Start = ConstraintMesh->GetComponentLocation();
	FVector End = Player->GetActorLocation();
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECC_GameTraceChannel2, CollisionQueryParams);

	BlackboardComponent->SetValueAsFloat("DistanceToTarget", Result.Distance);
	BlackboardComponent->SetValueAsObject("Target", Cast<ASPMCharacter>(Result.GetActor()) != nullptr && Result.Distance <= AttackRange ? Player : nullptr);
}

float ADrone::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
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
	LeftFire = !LeftFire;
	const FVector Origin = LeftFire ? WeaponLeft->GetComponentLocation() : WeaponRight->GetComponentLocation();
	FRotator Rotation = LeftFire ? WeaponLeft->GetComponentRotation() : WeaponRight->GetComponentRotation();
	Rotation.Pitch += FMath::RandRange(-AccuracyMargin, AccuracyMargin);
	Rotation.Roll += FMath::RandRange(-AccuracyMargin, AccuracyMargin);;
	Rotation.Yaw += FMath::RandRange(-AccuracyMargin, AccuracyMargin);;
	ADroneProjectile* NewProjectile = GetWorld()->SpawnActor<ADroneProjectile>(Projectile, Origin, Rotation);
	NewProjectile->SetOwner(this);
	NewProjectile->SetDamage(Damage);
	AmmoCount--;
}

void ADrone::Reload()
{
	AmmoCount = Ammo;
}



