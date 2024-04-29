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
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	WeaponBaseLeft = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponBaseLeft"));
	WeaponBaseLeft->SetupAttachment(RootComponent);
	ProjectileOriginLeft = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileOriginLeft"));
	ProjectileOriginLeft->SetupAttachment(WeaponBaseLeft);
	WeaponBaseRight = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponBaseRight"));
	WeaponBaseRight->SetupAttachment(RootComponent);
	ProjectileOriginRight = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileOriginRight"));
	ProjectileOriginRight->SetupAttachment(WeaponBaseRight);
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();
	
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
	FVector Start = RootComponent->GetComponentLocation();
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

	if(Height < 25 && Velocity.Z < 0)
	{
		Velocity.Z = 0;
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
	//TODO: Move from blueprint into c++
}

void ADrone::Shoot()
{
	LeftFire = !LeftFire;
	const FVector Origin = LeftFire ? ProjectileOriginLeft->GetComponentLocation() : ProjectileOriginRight->GetComponentLocation();
	FRotator Rotation = LeftFire ? WeaponBaseLeft->GetComponentRotation() : WeaponBaseRight->GetComponentRotation();
	Rotation += FRotator(FMath::RandRange(-AccuracyMargin, AccuracyMargin), FMath::RandRange(-AccuracyMargin, AccuracyMargin), FMath::RandRange(-AccuracyMargin, AccuracyMargin));
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