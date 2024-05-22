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
	FlyingMovement = CreateDefaultSubobject<UFlyingMovementComponent>(TEXT("FlyingMovement"));
	
	EnemyType = EDrone;
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	Ammo = MaxAmmo;
	Health = MaxHealth;
	FlyingMovement->OnLidarHit.AddDynamic(this, &ADrone::LidarHit);
	GetWorld()->GetGameInstance()->GetSubsystem<UMasterMindInstancedSubsystem>()->IncreasEnemyAmount(EDrone);
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADrone::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		FlyingMovement->SetPlayerTrail(Result.ImpactPoint + FVector(0, 0, 100));
	}

	else
	{
		Target = nullptr;
	}
}

FVector ADrone::GetKiteLocation() const
{
	if (!Player) return GetActorLocation();

	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), GetActorLocation());
	Rotation.Pitch = FMath::RandRange(0, AimPitch);
	Rotation.Yaw = Rotation.Yaw + (FMath::RandRange(KiteYaw / 2, KiteYaw) * (FMath::RandBool() ? -1 : 1));
	const FVector Direction = Rotation.RotateVector(FVector::ForwardVector);

	return Player->GetActorLocation() + Direction * (AttackRange - FMath::RandRange(0, KiteRange));
}

FVector ADrone::GetPatrolLocation() const
{
	const FRotator Rotation = FRotator(FMath::RandRange(-PatrolPitch, PatrolPitch),
	                                   FMath::RandRange(-PatrolYaw, PatrolYaw),
	                                   0);
	const FVector Direction = Rotation.RotateVector(FVector::ForwardVector);

	return GetActorLocation() + Direction * FMath::RandRange(PatrolMin, PatrolMax);
}

FVector ADrone::GetStrafeLocation(const int State) const
{
	if (!Player) return GetActorLocation();

	FRotator Rotation;

	switch (State)
	{
	case 0:
		Rotation = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), GetActorLocation());
		Rotation.Pitch = StrafePitch;
		return Player->GetActorLocation() + Rotation.RotateVector(FVector::ForwardVector) * StrafeDistance;

	case 1:
		Rotation = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), GetActorLocation());
		Rotation.Pitch = 0;
		return Player->GetActorLocation() + FVector(0, 0, StrafeOffset) + Rotation.RotateVector(FVector::ForwardVector) * -StrafeOffset;

	case 2:
		return GetKiteLocation();

	default:
		return GetActorLocation();
	}
}

void ADrone::AimAtPosition(const FVector Position) const
{
	const float Pitch = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Position).Pitch;
	SetAimPitch(FMath::Clamp(Pitch, -AimPitch, AimPitch));
}

void ADrone::SetAimPitch(const float Pitch) const
{
	WeaponBaseLeft->SetRelativeRotation(FRotator(Pitch, 0, 0));
	WeaponBaseRight->SetRelativeRotation(FRotator(Pitch, 0, 0));
}

void ADrone::Shoot(const bool UseNormalDamage)
{
	LeftFire = !LeftFire;
	const FVector Origin = LeftFire ? WeaponLookAtLeft->GetComponentLocation() : WeaponLookAtRight->GetComponentLocation();
	FRotator Rotation = LeftFire ? WeaponBaseLeft->GetComponentRotation() : WeaponBaseRight->GetComponentRotation();
	Rotation.Add(FMath::RandRange(-AccuracyMargin, AccuracyMargin),
	             FMath::RandRange(-AccuracyMargin, AccuracyMargin) + (LeftFire ? AimYawCorrection : -AimYawCorrection),
	             FMath::RandRange(-AccuracyMargin, AccuracyMargin));
	ADroneProjectile* NewProjectile = GetWorld()->SpawnActor<ADroneProjectile>(Projectile, Origin, Rotation);
	NewProjectile->SetOwner(this);
	NewProjectile->SetDamage(UseNormalDamage ? Damage : StrafeDamage);
	Ammo--;

	OnShoot(LeftFire);
}

void ADrone::Reload()
{
	Ammo = MaxAmmo;
}

float ADrone::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCauser) FlyingMovement->SetPlayerTrail(DamageCauser->GetActorLocation());

	float const TakenDamage = FMath::Min(Health, Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser));

	if ((Health -= TakenDamage) <= 0)
	{
		IsDead = true;
	}

	return TakenDamage;
}

bool ADrone::HasTarget() const
{
	return Target != nullptr;
}

void ADrone::LidarHit(const FHitResult &HitResult)
{
	if(IsStrafing && HitResult.GetActor() && !Cast<ASPMCharacter>(HitResult.GetActor()))
	{
		FlyingMovement->StopMove();
	}
}

void ADrone::OnShoot_Implementation(bool IsLeftFire)
{
}
