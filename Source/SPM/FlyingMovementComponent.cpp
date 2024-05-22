// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingMovementComponent.h"

#include "SPMCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UFlyingMovementComponent::UFlyingMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFlyingMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Destination = GetOwner()->GetActorLocation();
}

void UFlyingMovementComponent::OnComponentDestroyed(const bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	delete PlayerTrail;
}

void UFlyingMovementComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Rotate(DeltaTime);
	Move(DeltaTime);
}

void UFlyingMovementComponent::Rotate(const float DeltaTime)
{
	if (FVector::Distance(GetOwner()->GetActorLocation(), Destination) > StopDistance)
	{
		MovementDirection = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), Destination);
	}

	TargetRotation = Focus ? UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), Focus->GetActorLocation()) : MovementDirection;
	const FRotator Rotation = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
	GetOwner()->GetRootComponent()->SetWorldRotation(FRotator(0, Rotation.Yaw, 0));
}

void UFlyingMovementComponent::Move(const float DeltaTime)
{
	TargetVelocity.Normalize();
	const float Alpha = (FVector::Distance(GetOwner()->GetActorLocation(), Destination) - TargetEaseMargin) / TargetEaseDistance;
	TargetVelocity *= FMath::InterpEaseInOut(0, MovementSpeed, FMath::Clamp(Alpha, 0.0f, 1.0f), TargetEaseBlend);
	Velocity += (TargetVelocity - Velocity) * (FVector::DotProduct(TargetVelocity, Velocity) > 0.0f ? Acceleration : Deceleration) * DeltaTime;
	GetOwner()->GetRootComponent()->AddWorldOffset(Velocity * DeltaTime, true);
}

void UFlyingMovementComponent::MoveTo(const FVector Position, const int Speed, const int Stop)
{
	Destination = Position;
	MovementSpeed = Speed > 0 ? Speed : DefaultMovementSpeed;
	StopDistance = Stop > 0 ? Stop : DefaultStopDistance;
}

void UFlyingMovementComponent::StopMove()
{
	MoveTo(GetOwner()->GetActorLocation());
}

void UFlyingMovementComponent::SetFocus(AActor* NewFocus)
{
	Focus = NewFocus;
}

void UFlyingMovementComponent::ClearFocus()
{
	SetFocus(nullptr);
}

void UFlyingMovementComponent::GetTargetVelocity()
{
	TargetVelocity = FVector::Zero();

	for (const FRotator Direction : LidarDirections)
	{
		CheckLidarDirection(MovementDirection + Direction);
	}

	HasDestination = FVector::Distance(GetOwner()->GetActorLocation(), Destination) > StopDistance;

	if (Debug && HasDestination)
	{
		DrawDebugSphere(GetWorld(), Destination, 50, 8, FColor::Red, false, 1.0f);
	}
}

void UFlyingMovementComponent::CheckLidarDirection(FRotator Rotation)
{
	FVector Direction = Rotation.RotateVector(FVector::ForwardVector);
	FHitResult Result;
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + Direction * ObstacleAvoidanceDistance;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	if (Debug)
	{
		const FName TraceTag("DroneLidarLineTrace");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionQueryParams.TraceTag = TraceTag;
	}

	GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECC_Visibility, CollisionQueryParams);
	TargetVelocity += Result.bBlockingHit ? -Direction * ObstacleAvoidanceForce : Direction;
}

void UFlyingMovementComponent::SetPlayerTrail(const FVector Position)
{
	if (PlayerTrail) *PlayerTrail = Position;
	else PlayerTrail = new FVector(Position);
}

bool UFlyingMovementComponent::HasPlayerTrail() const
{
	return PlayerTrail != nullptr;
}

void UFlyingMovementComponent::ConsumePlayerTrail()
{
	if (PlayerTrail)
	{
		const FVector Position = FVector(*PlayerTrail);
		MoveTo(Position, 1000);
		delete PlayerTrail;
		PlayerTrail = nullptr;
	}
}