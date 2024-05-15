// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EnemieEnum.generated.h"

UENUM(BlueprintType)
enum EEnemies
{
	ESpider = 0 ,
	EDrone = 1 ,
	EWallbreaker = 2,
};

//Class to Create the Enemy Enums
UCLASS()
class UEnemiesEnum : public UObject
{
	GENERATED_BODY()
public:
	//UEnemiesEnum(const UEnemiesEnum& other);
	UEnemiesEnum();
	UEnemiesEnum(TEnumAsByte<EEnemies> EnemyType);
	~UEnemiesEnum();

	EEnemies Enemy;
	int MostTotalHits; 

	TEnumAsByte<EEnemies>  GetValue();
	
};

inline TArray<TEnumAsByte<EEnemies>> ListOfAllEnemieEnum; 
