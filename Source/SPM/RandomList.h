// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemieEnum.h"
#include "RandomList.generated.h"
/**
 * 
 */
UCLASS()
class SPM_API URandomList : public UObject
{
	GENERATED_BODY()
public:
	URandomList();
	~URandomList();


private:
	int8 Size = 10;
public:
	UPROPERTY()
	TArray<TEnumAsByte<EEnemies>> RandomListChance;

	TEnumAsByte<EEnemies> List[10];
		
	int8 SpiderLastIndex;
	int8 DroneLastIndex; 
	int8 WallbreakerLastIndex;

	void SetSize(int Amount){Size = Amount;};
	int Length(){return Size;};
};
