// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"

UFlyingMovementComponent::UFlyingMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFlyingMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialized destination
	Destination = GetOwner()->GetActorLocation();
}

void UFlyingMovementComponent::OnComponentDestroyed(const bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	// Cleans up pointer
	delete PlayerTrail;
}

void UFlyingMovementComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Rotation and movement
	Rotate(DeltaTime);
	Move(DeltaTime);
}

void UFlyingMovementComponent::Rotate(const float DeltaTime)
{
	if (FVector::Distance(GetOwner()->GetActorLocation(), Destination) > StopDistance)
	{
		// If we are far enough from the destination, we update our movement direction
		MovementDirection = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), Destination);
	}

	// If focus is set, we set target rotation as the location of focus, otherwise we set it as the movement direction
	// We then interpolate the current rotation and the target rotation for a smooth result
	TargetRotation = Focus ? UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), Focus->GetActorLocation()) : MovementDirection;
	const FRotator Rotation = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
	GetOwner()->GetRootComponent()->SetWorldRotation(FRotator(0, Rotation.Yaw, 0));
}

void UFlyingMovementComponent::Move(const float DeltaTime)
{
	// We normalize the target velocity as to be able to control the magnitude
	// We then get the alpha value for the interpolation, which increased the closer we are to the destination
	// This alpha is clamped between 0 and 1
	// We then take the difference between out current velocity and the target velocity and multiply this by an acceleration and delta time, as to approach it
	// The acceleration is determined by the dot product of the target velocity and the velocity,
	// as to set a higher acceleration if we have to make drastic changed in direction (deceleration)
	// The result of this is added to our velocity and out velocity is applied to the root of the owner of this component.
	TargetVelocity.Normalize();
	const float Alpha = (FVector::Distance(GetOwner()->GetActorLocation(), Destination) - TargetEaseMargin) / TargetEaseDistance;
	TargetVelocity *= FMath::InterpEaseInOut(0, MovementSpeed, FMath::Clamp(Alpha, 0.0f, 1.0f), TargetEaseBlend);
	Velocity += (TargetVelocity - Velocity) * (FVector::DotProduct(TargetVelocity, Velocity) > 0.0f ? Acceleration : Deceleration) * DeltaTime;
	GetOwner()->GetRootComponent()->AddWorldOffset(Velocity * DeltaTime, true);
}

void UFlyingMovementComponent::MoveTo(const FVector Position, const int Speed, const int Stop)
{
	// Set the destination, movement speed, and stop distance
	Destination = Position;
	MovementSpeed = Speed > 0 ? Speed : DefaultMovementSpeed;
	StopDistance = Stop > 0 ? Stop : DefaultStopDistance;
}

void UFlyingMovementComponent::StopMove()
{
	// Sets the destination to the current location, which will result in stop of movement
	MoveTo(GetOwner()->GetActorLocation());
}

void UFlyingMovementComponent::SetFocus(AActor* NewFocus)
{
	// Sets the focus
	Focus = NewFocus;
}

void UFlyingMovementComponent::ClearFocus()
{
	// Sets the focus to nullptr, as to clear it
	SetFocus(nullptr);
}

void UFlyingMovementComponent::GetTargetVelocity()
{
	// Resets the target velocity
	TargetVelocity = FVector::Zero();
	
	for (const FRotator Direction : LidarDirections)
	{
		// Iterates over each lidar direction and performs a lidar
		CheckLidarDirection(MovementDirection + Direction);
	}

	// If we are far enough from the destination we have a destination
	HasDestination = FVector::Distance(GetOwner()->GetActorLocation(), Destination) > StopDistance;

	if (Debug && HasDestination)
	{
		// Draws a sphere at the destination
		DrawDebugSphere(GetWorld(), Destination, 50, 8, FColor::Red, false, 1.0f);
	}
}

void UFlyingMovementComponent::CheckLidarDirection(FRotator Rotation)
{
	// Gets the direction as a vector by rotating a forward vector by the given rotation
	// Start location is the current position, and end location is the direction multiplied by a constant added to the start
	// Set to ignore collisions with the owner
	FVector Direction = Rotation.RotateVector(FVector::ForwardVector);
	FHitResult Result;
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + Direction * ObstacleAvoidanceDistance;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	if (Debug)
	{
		// Draws a line following the line trace
		const FName TraceTag("DroneLidarLineTrace");
		//GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionQueryParams.TraceTag = TraceTag;
	}

	// Performs the line trace in the visibility channel
	// A blocking hit means an obstacle could hinder the movement and the opposite
	// of the line trace direction is added to the target velocity acting as obstacle avoidance
	// Else there is nothing in the way, and we add the line trace direction to the target velocity
	GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECC_Visibility, CollisionQueryParams);
	TargetVelocity += Result.bBlockingHit ? -Direction * ObstacleAvoidanceForce : Direction;

	if(Result.bBlockingHit)
	{
		// Broadcasts that there was an obstacle detected
		OnLidarHit.Broadcast(Result);
	}
}

void UFlyingMovementComponent::SetPlayerTrail(const FVector Position)
{
	// Sets the player trail, if it is currently nullptr we create a new FVector with the value, else we set it to the new value
	if (PlayerTrail)
	{
		 *PlayerTrail = Position;
	}
	else
	{
		PlayerTrail = new FVector(Position);
	}
}

bool UFlyingMovementComponent::HasPlayerTrail() const
{
	// If the player trail is nullptr we don't have a player trail
	return PlayerTrail != nullptr;
}

void UFlyingMovementComponent::ConsumePlayerTrail()
{
	// If we have a player trail we will set the destination to it, and clear it
	if (PlayerTrail)
	{
		MoveTo(FVector(*PlayerTrail), 1000);
		delete PlayerTrail;
		PlayerTrail = nullptr;
	}
}