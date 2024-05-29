// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HazardousWater.generated.h"

class UDamageComponent;
class UBoxComponent;

UCLASS()
class SPM_API AHazardousWater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHazardousWater();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeUntilDamage;

	float Timer;
	
	UPROPERTY(EditAnywhere)
	UDamageComponent* DamageComponent;
	//The HazardousWater
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Water;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* WaterCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPlayerOnWater;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* Audio;

	UPROPERTY(BlueprintReadWrite)
	ACharacter* Player;
	
	UFUNCTION(BlueprintCallable)
	void DamageActor();

	UFUNCTION(BlueprintCallable)
	void SetMaxAcceleration(float Amount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlowDownAmount = 400;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float OriginalSpeed; 





};
