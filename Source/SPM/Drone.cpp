// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"

#include "DroneProjectile.h"
#include "SPMCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

ADrone::ADrone()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	RootComponent = Root;
	
	StableMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StableMesh"));
	StableMesh->SetupAttachment(Root);
	
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
	MoveTo( GetActorLocation());
	AmmoCount = Ammo;

	BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(this);
	BlackboardComponent->SetValueAsFloat("AttackSpeed", AttackSpeed);
	BlackboardComponent->SetValueAsFloat("ReloadSpeed", ReloadSpeed);
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADrone::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckLineOfSightAtPlayer();
	GetMovementDirection();
	GetHoverHeight();
	Movement(DeltaTime);
}

void ADrone::CheckLineOfSightAtPlayer() const
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

void ADrone::GetMovementDirection()
{
	FVector Direction = TargetPosition - GetActorLocation();
	
	if(Direction.Length() < StopDistance)
	{
		TargetVelocity.X = 0;
		TargetVelocity.Y = 0;
		return;
	}
	
	Direction.Normalize();
	TargetVelocity.X = Direction.X * MovementSpeed;
	TargetVelocity.Y = Direction.Y * MovementSpeed;
	LookAt(TargetPosition);
}

void ADrone::GetHoverHeight()
{
	FHitResult Result;
	FVector Start = RootComponent->GetComponentLocation() + FVector(0, 0, 0);
	FVector End = Start - FVector(0, 0, DefaultHoverHeight * 10);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECC_Visibility, CollisionQueryParams);
	Height = Result.Distance;
	const float Difference = TargetHeight - Height;
	
	if(Difference > HoverMargin) TargetVelocity.Z = Difference * HoverSpeed;
	else if(Difference < HoverMargin) TargetVelocity.Z = Difference * HoverSpeed;
	else TargetVelocity.Z = 0;
}

void ADrone::Movement(const float DeltaTime)
{
	if(const FVector Step = TargetVelocity - Velocity; Step.Length() > 1)
	{
		Velocity += Step * (Acceleration * DeltaTime);
	}

	Root->AddWorldOffset(Velocity * DeltaTime);
	Root->SetWorldRotation(TargetRotation);
}

void ADrone::MoveTo(const FVector Position)
{
	TargetPosition = Position;
	TargetHeight = DefaultHoverHeight;
}

void ADrone::LookAt(const FVector Position)
{
	FRotator Rotation = (Position - GetActorLocation()).Rotation();
	Rotation.Pitch = 0;
	TargetRotation = Rotation;
}

void ADrone::Aim(const FVector Position) const
{
	FRotator Rotation = (Position - GetActorLocation()).GetSafeNormal().Rotation();
	Rotation.Roll = 0;
	Rotation.Yaw = 0;
	WeaponLeft->SetRelativeRotation(Rotation);
	WeaponRight->SetRelativeRotation(Rotation);
}

void ADrone::Shoot()
{
	LeftFire = !LeftFire;
	const FVector Origin = LeftFire ? WeaponLeft->GetComponentLocation() : WeaponRight->GetComponentLocation();
	FRotator Rotation = LeftFire ? WeaponLeft->GetComponentRotation() : WeaponRight->GetComponentRotation();
	Rotation.Pitch += FMath::RandRange(-AccuracyMargin, AccuracyMargin);
	Rotation.Roll += FMath::RandRange(-AccuracyMargin, AccuracyMargin);
	Rotation.Yaw += FMath::RandRange(-AccuracyMargin, AccuracyMargin) + (LeftFire ? 4 : -4);
	ADroneProjectile* NewProjectile = GetWorld()->SpawnActor<ADroneProjectile>(Projectile, Origin, Rotation);
	NewProjectile->SetOwner(this);
	NewProjectile->SetDamage(Damage);
	AmmoCount--;
}

void ADrone::Reload()
{
	AmmoCount = Ammo;
}

float ADrone::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float const TakenDamage = FMath::Min(Health, Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser));;
	
	if((Health -= TakenDamage) <= 0)
	{
		GetController()->Destroy();
		Destroy();
	}
	
	return TakenDamage;
}