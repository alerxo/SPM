// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemieEnum.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyInfo.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FEnemyStats
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	int Killed = 0;
	UPROPERTY(BlueprintReadWrite)
	int Amount = 0;
	UPROPERTY(BlueprintReadWrite)
	int TotalHits = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	double Weight = 0.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int TokenCost = 0;
	UPROPERTY(BlueprintReadWrite)
	float DamageAmount = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EEnemies> EnemyType = ESpider;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<APawn> Enemy = NULL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBehaviorTree* EnemyTree = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxRange = 0.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinRange = 0.0;
};
