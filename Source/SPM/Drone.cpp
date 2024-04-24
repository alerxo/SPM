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
	BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(this);
	BlackboardComponent->SetValueAsFloat("AttackSpeed", AttackSpeed);
	BlackboardComponent->SetValueAsFloat("ReloadSpeed", ReloadSpeed);
	AmmoCount = Ammo;
	Destination = GetActorLocation();
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADrone::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	TryLookAtPlayer();
	MoveTowardsDestination();
	ObstacleAvoidance();
	Movement(DeltaTime);
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

void ADrone::MoveTowardsDestination()
{
	if(FVector::Distance(Destination, GetActorLocation()) < 0.1f)
	{
		TargetVelocity.X = 0;
		TargetVelocity.Y = 0;
		return;
	}
	
	FVector Direction = Destination - GetActorLocation();
	Direction.Z = 0;
	Direction.Normalize();
	TargetVelocity.X = Direction.X * MovementSpeed;
	TargetVelocity.Y = Direction.Y * MovementSpeed;
}

void ADrone::SetDestination(const FVector Position)
{
	Destination = Position;
}

void ADrone::ObstacleAvoidance()
{
	FHitResult Result;
	FVector Start = RootComponent->GetComponentLocation() + FVector(0, 0, 0);
	FVector End = Start - FVector(0, 0, TargetHeight * 10);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	const FName TraceTag("MyTraceTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	CollisionQueryParams.TraceTag = TraceTag;
	
	GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECC_Visibility, CollisionQueryParams);
	Height = Result.Distance;
	
	float ZVelocity = 0;
	if(Height > TargetHeight + HoverMargin) ZVelocity = -HoverSpeed;
	else if(Height < TargetHeight - HoverMargin) ZVelocity = HoverSpeed;
	
	TargetVelocity.Z = ZVelocity;
}

void ADrone::Movement(const float DeltaTime) const
{
	//if(Velocity.Length() < 0.01) return;
	Root->AddWorldOffset(TargetVelocity * DeltaTime);
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