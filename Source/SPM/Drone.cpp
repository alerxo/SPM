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

	// Initializes player, ammo, health and broadcasts to the mastermind that a drone has spawned
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

	// Line trace that starts at the drone location and ends at the player
	// Set to ignore the current drone
	FHitResult Result;
	FVector Start = GetActorLocation();
	FVector End = Player->GetActorLocation();
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	if (Debug)
	{
		// Draws a line following the line trace
		const FName TraceTag("DroneSightLineTrace");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionQueryParams.TraceTag = TraceTag;
	}

	// Performs the line trace and sets the distance to the target
	GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECC_GameTraceChannel2, CollisionQueryParams);
	DistanceToTarget = Result.Distance;

	if (Result.bBlockingHit && Cast<ASPMCharacter>(Result.GetActor()) && Result.Distance <= ChaseRange)
	{
		// If the line trace was blocked by the player and the distance to the player is smaller than the chase range
		// The target is set to the player, we broadcast to the mastermind that we have found the player, and we set the player trail to is location
		Target = Result.GetActor();
		GetWorld()->GetGameInstance()->GetSubsystem<UMasterMindInstancedSubsystem>()->OnPlayerSeen.Broadcast(GetActorLocation());
		FlyingMovement->SetPlayerTrail(Result.ImpactPoint + FVector(0, 0, 100));
	}
	else
	{
		// Something was blocking the line of sight at the player which means we don't have a target
		Target = nullptr;
	}
}

FVector ADrone::GetKiteLocation() const
{
	if (!Player) return GetActorLocation();

	// Gets the look at rotation from the player to the drone, adds random pitch and yaw to this,
	// and multiplies it by the attack range and random float withing the kite range 
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), GetActorLocation());
	Rotation.Pitch = FMath::RandRange(0, AimPitch);
	Rotation.Yaw = Rotation.Yaw + (FMath::RandRange(KiteYaw / 2, KiteYaw) * (FMath::RandBool() ? -1 : 1));
	const FVector Direction = Rotation.RotateVector(FVector::ForwardVector);

	return Player->GetActorLocation() + Direction * (AttackRange - FMath::RandRange(0, KiteRange));
}

FVector ADrone::GetPatrolLocation() const
{
	// Gets a random rotation withing the given range, gets a random distance withing the given range and returns it
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
		// The first state of the strafe, gets a location above the player and with enough space between the player and the drone
		Rotation = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), GetActorLocation());
		Rotation.Pitch = StrafePitch;
		return Player->GetActorLocation() + Rotation.RotateVector(FVector::ForwardVector) * StrafeDistance;

	case 1:
		// The second state of the strafe, gets the location behind the player in relation to the drone
		Rotation = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), GetActorLocation());
		Rotation.Pitch = 0;
		return Player->GetActorLocation() + FVector(0, 0, StrafeOffset) + Rotation.RotateVector(FVector::ForwardVector) * -StrafeOffset;

	case 2:
		// The last state of the strafe, gets a kite location
		return GetKiteLocation();

	default:
		return GetActorLocation();
	}
}

void ADrone::AimAtPosition(const FVector Position) const
{
	// Sets the pitch of the weapons to aim at the position, clamping it withing the accepted ranges
	const float Pitch = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Position).Pitch;
	SetAimPitch(FMath::Clamp(Pitch, -AimPitch, AimPitch));
}

void ADrone::SetAimPitch(const float Pitch) const
{
	// Sets the pitch of the weapons
	WeaponBaseLeft->SetRelativeRotation(FRotator(Pitch, 0, 0));
	WeaponBaseRight->SetRelativeRotation(FRotator(Pitch, 0, 0));
}

void ADrone::Shoot(const bool UseNormalDamage)
{
	// Alternates between the left weapon and the right weapon,
	// Gets the origin and rotation of the bullet by components in the actor blueprint,
	// adds some random rotation withing the accuracy value and adjust the yaw of the to help mitigate the position offset of the weapons 
	LeftFire = !LeftFire;
	const FVector Origin = LeftFire ? WeaponLookAtLeft->GetComponentLocation() : WeaponLookAtRight->GetComponentLocation();
	FRotator Rotation = LeftFire ? WeaponBaseLeft->GetComponentRotation() : WeaponBaseRight->GetComponentRotation();
	Rotation.Add(FMath::RandRange(-AccuracyMargin, AccuracyMargin),
	             FMath::RandRange(-AccuracyMargin, AccuracyMargin) + (LeftFire ? AimYawCorrection : -AimYawCorrection),
	             FMath::RandRange(-AccuracyMargin, AccuracyMargin));

	// Spawns a new projectile and sets it owner and damage, reduces ammo count
	ADroneProjectile* NewProjectile = GetWorld()->SpawnActor<ADroneProjectile>(Projectile, Origin, Rotation);
	NewProjectile->SetOwner(this);
	NewProjectile->SetDamage(UseNormalDamage ? Damage : StrafeDamage);
	Ammo--;

	// Calls the fire event
	OnShoot(LeftFire);
}

void ADrone::Reload()
{
	// Sets ammo to the max
	Ammo = MaxAmmo;
}

float ADrone::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float const TakenDamage = FMath::Min(Health, Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser));

	if (DamageCauser)
	{
		// Sets the player trail to the location of the owner of the incoming damage
		FlyingMovement->SetPlayerTrail(DamageCauser->GetActorLocation());
	}
	
	if ((Health -= TakenDamage) <= 0)
	{
		// If we have fallen below 0 health we are dead
		IsDead = true;
	}

	return TakenDamage;
}

bool ADrone::HasTarget() const
{
	// We have a target if it is not nullptr
	return Target != nullptr;
}

void ADrone::LidarHit(const FHitResult &HitResult)
{
	if(IsStrafing && HitResult.GetActor() && !Cast<ASPMCharacter>(HitResult.GetActor()))
	{
		// If we are strafing and an obstacle was found in our way, we cancel the strafe by stopping movement
		FlyingMovement->StopMove();
	}
}

void ADrone::OnShoot_Implementation(bool IsLeftFire)
{
}