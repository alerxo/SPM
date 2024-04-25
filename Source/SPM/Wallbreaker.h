// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Wallbreaker.generated.h"

UCLASS()
class SPM_API AWallbreaker : public ACharacter
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
	float GetAcceptanceRadius();

	UFUNCTION(BlueprintCallable)
	void SetAcceptanceRadius(float NewAcceptanceRadius);

	UFUNCTION(BlueprintCallable)
	float GetMaxWalkSpeed();
private:
	bool IsDead() const;
	float AcceptanceRadius = 1000;
	float MaxWalkSpeed;
};
