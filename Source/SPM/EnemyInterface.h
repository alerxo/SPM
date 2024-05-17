// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemieEnum.h"
#include "EnemyInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class IEnemyInterface
{
	GENERATED_BODY()
public:
	void SpawnHealthPickup();
	
	bool CheckToSpawnPickup() const;

	bool IsDead() const;
	
	TEnumAsByte<EEnemies> EnemyType; 
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Investigate(FVector Location);


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	EEnemies GetEnemyTyp();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayerSeen();
};
