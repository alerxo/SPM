// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"

#include "DroneProjectile.h"
#include "MasterMindInstancedSubsystem.h"
#include "SPMCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ADrone::ADrone()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	RootComponent = Root;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	WeaponBaseLeft = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponBaseLeft"));
	WeaponBaseLeft->SetupAttachment(RootComponent);
	WeaponLookAtLeft = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponLookAtLeft"));
	WeaponLookAtLeft->SetupAttachment(WeaponBaseLeft);
	WeaponBaseRight = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponBaseRight"));
	WeaponBaseRight->SetupAttachment(RootComponent);
	WeaponLookAtRight = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponLookAtRight"));
	WeaponLookAtRight->SetupAttachment(WeaponBaseRight);
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	Destination = GetActorLocation();
	Ammo = MaxAmmo;
	Health = MaxHealth;
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADrone::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rotate(DeltaTime);
	Move(DeltaTime);
}

void ADrone::CheckLineOfSightAtPlayer()
{
	if (!Player) return;

	FHitResult Result;
	FVector Start = GetActorLocation();
	FVector End = Player->GetActorLocation();
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	if (Debug)
	{
		const FName TraceTag("DroneSightLineTrace");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionQueryParams.TraceTag = TraceTag;
	}

	GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECC_GameTraceChannel2, CollisionQueryParams);
	DistanceToTarget = Result.Distance;

	if (Result.bBlockingHit && Cast<ASPMCharacter>(Result.GetActor()) && Result.Distance <= ChaseRange)
	{
		Target = Result.GetActor();
		GetWorld()->GetGameInstance()->GetSubsystem<UMasterMindInstancedSubsystem>()->OnPlayerSeen.Broadcast(GetActorLocation());
	}

	else
	{
		Target = nullptr;
		IsInCombat = false;
	}
}

void ADrone::Rotate(const float DeltaTime)
{
	if (FVector::Distance(GetActorLocation(), Destination) > StopDistance)
	{
		MovementDirection = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Destination);
	}

	TargetRotation = Target ? UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation()) : MovementDirection;
	const FRotator Rotation = UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
	Root->SetWorldRotation(FRotator(0, Rotation.Yaw, 0));
}

void ADrone::GetTargetVelocity()
{
	TargetVelocity = FVector::Zero();

	for (const FRotator Direction : LidarDirections)
	{
		CheckLidarDirection(MovementDirection + Direction);
	}

	HasDestination = FVector::Distance(GetActorLocation(), Destination) > StopDistance;

	if (Debug && HasDestination)
	{
		DrawDebugSphere(GetWorld(), Destination, 50, 8, FColor::Red, false, 1.0f);
	}
}

void ADrone::CheckLidarDirection(FRotator Rotation)
{
	FVector Direction = Rotation.RotateVector(FVector::ForwardVector);
	FHitResult Result;
	FVector Start = GetActorLocation();
	FVector End = Start + Direction * ObstacleAvoidanceDistance;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	if (Debug)
	{
		const FName TraceTag("DroneLidarLineTrace");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionQueryParams.TraceTag = TraceTag;
	}

	GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECC_Visibility, CollisionQueryParams);
	TargetVelocity += Result.bBlockingHit ? -Direction * ObstacleAvoidanceForce : Direction;
}

void ADrone::Move(const float DeltaTime)
{
	TargetVelocity.Normalize();
	const float Alpha = (FVector::Distance(GetActorLocation(), Destination) - TargetEaseMargin) / TargetEaseDistance;
	TargetVelocity *= FMath::InterpEaseInOut(0, MovementSpeed, FMath::Clamp(Alpha, 0.0f, 1.0f), TargetEaseBlend);
	Velocity += (TargetVelocity - Velocity) * (FVector::DotProduct(TargetVelocity, Velocity) > 0.0f ? Acceleration : Deceleration) * DeltaTime;
	Root->AddWorldOffset(Velocity * DeltaTime, true);
}

void ADrone::MoveTo(const FVector Position)
{
	Destination = Position;
}

FVector ADrone::GetKiteLocation() const
{
	if (!Player) return GetActorLocation();

	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), GetActorLocation());
	Rotation.Pitch = FMath::RandRange(0, AimPitch),
		Rotation.Yaw = Rotation.Yaw + FMath::RandRange(-KiteYawDegree, KiteYawDegree);
	const FVector Direction = Rotation.RotateVector(FVector::ForwardVector);

	return Player->GetActorLocation() + Direction * (AttackRange - FMath::RandRange(0, KiteRange));;
}

FVector ADrone::GetPatrolLocation() const
{
	const FRotator Rotation = FRotator(FMath::RandRange(-PatrolPitch, PatrolPitch),
	                                   FMath::RandRange(-PatrolYaw, PatrolYaw),
	                                   0);
	const FVector Direction = Rotation.RotateVector(FVector::ForwardVector);

	return GetActorLocation() + Direction * FMath::RandRange(500, 1000);;
}

void ADrone::Aim(const FVector Position) const
{
	float Pitch = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Position).Pitch;
	Pitch = FMath::Clamp(Pitch, -AimPitch, AimPitch);
	WeaponBaseLeft->SetRelativeRotation(FRotator(Pitch, 0, 0));
	WeaponBaseRight->SetRelativeRotation(FRotator(Pitch, 0, 0));
}

void ADrone::Shoot()
{
	IsInCombat = true;
	LeftFire = !LeftFire;
	const FVector Origin = LeftFire ? WeaponLookAtLeft->GetComponentLocation() : WeaponLookAtRight->GetComponentLocation();
	FRotator Rotation = LeftFire ? WeaponBaseLeft->GetComponentRotation() : WeaponBaseRight->GetComponentRotation();
	Rotation.Add(FMath::RandRange(-AccuracyMargin, AccuracyMargin),
	             FMath::RandRange(-AccuracyMargin, AccuracyMargin) + (LeftFire ? AimYawCorrection : -AimYawCorrection),
	             FMath::RandRange(-AccuracyMargin, AccuracyMargin));
	ADroneProjectile* NewProjectile = GetWorld()->SpawnActor<ADroneProjectile>(Projectile, Origin, Rotation);
	NewProjectile->SetOwner(this);
	NewProjectile->SetDamage(Damage);
	Ammo--;

	OnShoot(LeftFire);
}

void ADrone::Reload()
{
	Ammo = MaxAmmo;
}

float ADrone::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	IsInCombat = true;
	float const TakenDamage = FMath::Min(Health, Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser));

	if ((Health -= TakenDamage) <= 0)
	{
		GetController()->Destroy();
		Destroy();
	}

	return TakenDamage;
}

bool ADrone::HasTarget() const
{
	return Target != nullptr;
}

void ADrone::OnShoot_Implementation(bool IsLeftFire)
{
}
