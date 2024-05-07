// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"

#include "DroneProjectile.h"
#include "MasterMindInstancedSubsystem.h"
#include "SPMCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
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
	MoveTo(GetActorLocation());
	Reload();
	BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(this);
	
	if(BlackboardComponent)
	{
		BlackboardComponent->SetValueAsFloat("AttackSpeed", AttackSpeed);
		BlackboardComponent->SetValueAsFloat("ReloadSpeed", ReloadSpeed);
	}
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADrone::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(++TickCount == TickInterval)
	{
		CheckLineOfSightAtPlayer();
		Rotate();
		GetTargetVelocity();
		TickCount = 0;
	}

	Move(DeltaTime);
}

void ADrone::CheckLineOfSightAtPlayer()
{
	if (!Player) return;

	FHitResult Result;
	FVector Start = RootComponent->GetComponentLocation();
	FVector End = Player->GetActorLocation();
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECC_GameTraceChannel2, CollisionQueryParams);
	BlackboardComponent->SetValueAsFloat("DistanceToTarget", Result.Distance);

	AActor* HitActor = Cast<ASPMCharacter>(Result.GetActor());

	if (HitActor && IsInCombat ? true : Result.Distance <= AttackRange + KiteRange)
	{
		BlackboardComponent->SetValueAsObject("Target", HitActor);
		GetWorld()->GetGameInstance()->GetSubsystem<UMasterMindInstancedSubsystem>()->OnPlayerSeen.Broadcast(GetActorLocation());
	}

	else
	{
		BlackboardComponent->SetValueAsObject("Target", nullptr);
		IsInCombat = false;
	}
}

void ADrone::Rotate()
{
	if (FVector::Distance(Root->GetComponentLocation(), Destination) > StopDistance)
	{
		MovementDirection = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Destination);
	}

	TargetRotation = Focus ? UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Focus->GetActorLocation()) : MovementDirection;

	FRotator Rotation = TargetRotation;
	Rotation.Pitch = 0;
	Root->SetWorldRotation(Rotation);
}

void ADrone::GetTargetVelocity()
{
	TargetVelocity = FVector::Zero();

	if (FVector::Distance(Root->GetComponentLocation(), Destination) > StopDistance)
	{
		for (const FRotator Direction : LidarDirections)
		{
			CheckLidarDirection(MovementDirection + Direction);
		}

		HasDestination = true;
		return;
	}

	HasDestination = false;
}

void ADrone::CheckLidarDirection(FRotator Rotation)
{
	FVector Direction = Rotation.RotateVector(FVector::ForwardVector);
	FHitResult Result;
	FVector Start = RootComponent->GetComponentLocation();
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
	TargetVelocity += FMath::Lerp(Direction, -Direction * ObstacleAvoidanceForce, Result.Distance / ObstacleAvoidanceDistance);
}

void ADrone::Move(const float DeltaTime)
{
	TargetVelocity.Normalize();
	TargetVelocity *= MovementSpeed;

	if (const FVector Step = TargetVelocity - Velocity; Step.Length() > 1)
	{
		Velocity += Step * (Acceleration * DeltaTime);
	}

	Root->AddWorldOffset(Velocity * DeltaTime, true);
}

void ADrone::MoveTo(const FVector Position)
{
	Destination = Position;
}

FVector ADrone::GetKiteLocation() const
{
	if (!Player) return GetActorLocation();

	FVector Location = (GetActorLocation() - Player->GetActorLocation()).GetSafeNormal() * AttackRange;
	Location += Player->GetActorLocation();
	Location.Z = Player->GetActorLocation().Z;
	Location += FVector(FMath::RandRange(KiteRange / 2, KiteRange) * (FMath::RandBool() ? -1 : 1),
	                    FMath::RandRange(KiteRange / 2, KiteRange) * (FMath::RandBool() ? -1 : 1),
	                    FMath::RandRange(50, KiteRange));
	return Location;
}

void ADrone::SetFocus(AActor* Target)
{
	Focus = Target;
}

void ADrone::ClearFocus()
{
	Focus = nullptr;
}

void ADrone::Aim(const FVector Position) const
{
	float Pitch = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Position).Pitch;
	Pitch = FMath::Clamp(Pitch, -25, 25);
	WeaponBaseLeft->SetRelativeRotation(FRotator(Pitch, 0, 0));
	WeaponBaseRight->SetRelativeRotation(FRotator(Pitch, 0, 0));
}

void ADrone::Shoot()
{
	EnterCombat();
	LeftFire = !LeftFire;
	const FVector Origin = LeftFire ? WeaponLookAtLeft->GetComponentLocation() : WeaponLookAtRight->GetComponentLocation();
	FRotator Rotation = LeftFire ? WeaponBaseLeft->GetComponentRotation() : WeaponBaseRight->GetComponentRotation();
	Rotation.Add(FMath::RandRange(-AccuracyMargin, AccuracyMargin),
	             FMath::RandRange(-AccuracyMargin, AccuracyMargin) + (LeftFire ? 4 : -4),
	             FMath::RandRange(-AccuracyMargin, AccuracyMargin));
	ADroneProjectile* NewProjectile = GetWorld()->SpawnActor<ADroneProjectile>(Projectile, Origin, Rotation);
	NewProjectile->SetOwner(this);
	NewProjectile->SetDamage(Damage);
	AmmoCount--;

	OnShoot(LeftFire);
}

void ADrone::Reload()
{
	if(AmmoCount < Ammo)
	{
		AmmoCount = Ammo;
	}
}

float ADrone::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	EnterCombat();
	float const TakenDamage = FMath::Min(Health, Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser));
	
	if ((Health -= TakenDamage) <= 0)
	{
		GetController()->Destroy();
		Destroy();
	}

	return TakenDamage;
}

void ADrone::EnterCombat()
{
	IsInCombat = true;
}

void ADrone::OnShoot_Implementation(bool IsLeftFire)
{
}