// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseClass.h"
#include "Wallbreaker.generated.h"

UCLASS()
class SPM_API AWallbreaker : public AEnemyBaseClass
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWallbreaker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	float GetAcceptableDistance();

	UFUNCTION(BlueprintCallable)
	float GetMaxWalkSpeed();
private:

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	float AcceptableDistance = 2000;
	
	float MaxWalkSpeed;
};
