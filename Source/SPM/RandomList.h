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


	const int8 Size = 10;

	UPROPERTY()
	TArray<TEnumAsByte<EEnemies>> RandomListChance;

	TEnumAsByte<EEnemies> List[10]{ESpider,ESpider,ESpider,ESpider,ESpider,ESpider,ESpider,ESpider,ESpider,ESpider};
		
	int8 SpiderLastIndex;
	int8 DroneLastIndex; 
	int8 WallbreakerLastIndex;
};
