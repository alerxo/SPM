// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"

#include "DroneProjectile.h"
#include "SPMCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

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
	Rotate();
	GetMovementDirection();
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

void ADrone::Rotate() const
{
	Root->SetWorldRotation(TargetRotation);
}

void ADrone::GetMovementDirection()
{
	TargetVelocity = FVector::Zero();

	if(FVector::Distance(Root->GetComponentLocation(), Destination) < StopDistance)
	{
		return;
	}
	
	const TArray LidarDirections = 
	{
		RootComponent->GetUpVector(),
		-RootComponent->GetUpVector(),
		RootComponent->GetForwardVector(),
		RootComponent->GetRightVector(),
		-RootComponent->GetRightVector(),
	};
	
	for (const FVector LidarDirection : LidarDirections)
	{
		CheckLidarDirection(LidarDirection);
	}
}

void ADrone::CheckLidarDirection(FVector Direction)
{
	FHitResult Result;
	FVector Start = RootComponent->GetComponentLocation();
	FVector End = Start + (Direction * ObstacleAvoidanceDistance);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	if(Debug)
	{
		const FName TraceTag("DroneLidarLineTrace");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionQueryParams.TraceTag = TraceTag;
	}
	
	GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECC_Visibility, CollisionQueryParams);
	TargetVelocity += Direction * (ObstacleAvoidanceDistance - Result.Distance);
}

void ADrone::Movement(const float DeltaTime)
{
	TargetVelocity.Normalize();
	TargetVelocity *= MovementSpeed;
	
	if(const FVector Step = TargetVelocity - Velocity; Step.Length() > 1)
	{
		Velocity += Step * (Acceleration * DeltaTime);
	}
	
	Root->AddWorldOffset(Velocity * DeltaTime);
}

void ADrone::MoveTo(const FVector Position)
{
	Destination = Position;
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