// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyInterface.h"
#include "GameFramework/Character.h"
#include "HealthPickup.h"
#include "NiagaraSystem.h"
#include "EnemyBaseClass.generated.h"


UCLASS()
class SPM_API AEnemyBaseClass : public ACharacter, public IEnemyInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AEnemyBaseClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	bool IsDead_Implementation() const;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHealthPickup> HealthPickupClass;

	UPROPERTY(EditDefaultsOnly)
	float ChanceToSpawnPickup;

	UPROPERTY(EditDefaultsOnly)
	float ChanceToSpawnPickupLowHP;

	UPROPERTY(EditDefaultsOnly)
	float LowHealthThreshold;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> DeathNiagara;

	bool CheckToSpawnPickup_Implementation() const;

	void SpawnHealthPickup_Implementation();

};
