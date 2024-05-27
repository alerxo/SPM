// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MusicMaster.generated.h"

/**
 * 
 */

#define MAXINTENSITY 90
#define MEDIUMINTENSITY 50
#define LOWINTENSITY 20

enum IntensityValues
{
	LowIntensity,
	MediumIntensity,
	HighIntensity,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMaxIntensity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMediumIntensity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLowIntensity);
UCLASS()
class SPM_API UMusicMaster : public UObject
{
	GENERATED_BODY()

private:
	int IntensityMeter = 0;

	int MaximumIntensity = 100;

	IntensityValues CurrentIntensity;

public:

	UPROPERTY(BlueprintAssignable)
	FOnMaxIntensity OnMaxIntensity;
	UPROPERTY(BlueprintAssignable)
	FOnMediumIntensity OnMediumIntensity;
	UPROPERTY(BlueprintAssignable)
	FOnLowIntensity OnLowIntensity;
	
	

	
	UFUNCTION(BlueprintCallable)
	int& GetIntensityMeter(){return IntensityMeter;};
	UFUNCTION(BlueprintCallable)
	void LowerIntensityMeter(int Amount);
	UFUNCTION(BlueprintCallable)
	void IncreaseIntensityMeter(int Amount);

	
	
};
