// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlyingMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UFlyingMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFlyingMovementComponent();

protected:
	virtual void BeginPlay() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void MoveTo(const FVector Position, const int Speed = -1, const int Stop = -1);
	UFUNCTION(BlueprintCallable)
	void StopMove();
	UFUNCTION(BlueprintCallable)
	void SetFocus(AActor* NewFocus);
	UFUNCTION(BlueprintCallable)
	void ClearFocus();
	UFUNCTION(BlueprintCallable)
	void SetPlayerTrail(FVector Position);
	UFUNCTION(BlueprintCallable)
	bool HasPlayerTrail() const;
	UFUNCTION(BlueprintCallable)
	void ConsumePlayerTrail();
	UFUNCTION(BlueprintCallable)
	void GetTargetVelocity();
	
private:
	void Rotate(float);
	void CheckLidarDirection(FRotator);
	void Move(const float);
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool HasDestination = false;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector Velocity;

	UPROPERTY(EditAnywhere)
	bool Debug = false;

private:
	UPROPERTY()
	AActor* Focus = nullptr;
	FVector* PlayerTrail = nullptr;

	FRotator MovementDirection;
	FVector Destination;
	FVector TargetVelocity;
	FRotator TargetRotation;

	const int DefaultMovementSpeed = 700;
	int MovementSpeed = DefaultMovementSpeed;
	const float TargetEaseDistance = 200.0f;
	const float TargetEaseMargin = 10.0f;
	const float TargetEaseBlend = 5.0f;
	const float Acceleration = 1.0f;
	const float Deceleration = 2.0f;
	const int DefaultStopDistance = 100;
	int StopDistance = DefaultStopDistance;
	const int ObstacleAvoidanceDistance = 200;
	const float ObstacleAvoidanceForce = 20.0f;
	const float RotationSpeed = 4.0f;

	const TArray<FRotator> LidarDirections =
{
		FRotator(0, 0, 0),
		FRotator(30, 30, 0),
		FRotator(30, -30, 0),
		FRotator(-30, 30, 0),
		FRotator(-30, -30, 0),
		FRotator(90, 0, 0),
		FRotator(-90, 0, 0)
	};
};
