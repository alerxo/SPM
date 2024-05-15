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
UCLASS(BlueprintType)
class UEnemiesEnum : public UObject
{
	GENERATED_BODY()
public:
	//UEnemiesEnum(const UEnemiesEnum& other);
	UEnemiesEnum();
	UEnemiesEnum(TEnumAsByte<EEnemies> EnemyType);

	static UEnemiesEnum* Instiantiate(TEnumAsByte<EEnemies> EnemyType);
	~UEnemiesEnum();

	UPROPERTY()
	TEnumAsByte<EEnemies> Enemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MostTotalHits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Weight;
	UFUNCTION()
	TEnumAsByte<EEnemies>  GetValue();
	
	/*
	UPROPERTY()
	TArray<UEnemiesEnum*> ListOfAllEnemieEnum; 
	*/
};



