// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MusicMaster.generated.h"

/**
 * 
 */

#define MAXINTENSITY 5
#define MEDIUMINTENSITY 2
#define LOWINTENSITY 1

enum IntensityValues
{
	LowIntensity,
	MediumIntensity,
	HighIntensity,
};

DECLARE_DYNAMIC_DELEGATE(FCaller);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMaxIntensity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMediumIntensity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLowIntensity);
UCLASS()
class SPM_API UMusicMaster : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

private:
	int IntensityMeter = 0;

	int MaximumIntensity = 100;

	IntensityValues CurrentIntensity;

	UFUNCTION()
	void CallMax(){OnMaxIntensity.Broadcast();}
	UFUNCTION()
	void CallMed(){OnMediumIntensity.Broadcast();}
	UFUNCTION()
	void CallLow(){OnLowIntensity.Broadcast();}

public:

	UPROPERTY(BlueprintAssignable)
	FOnMaxIntensity OnMaxIntensity;
	UPROPERTY(BlueprintAssignable)
	FOnMediumIntensity OnMediumIntensity;
	UPROPERTY(BlueprintAssignable)
	FOnLowIntensity OnLowIntensity;


	UPROPERTY()
	FCaller Caller;
	
	float Timer = 10; 

	void ResetTimer();
	void StartTimer();

	bool bIsTransitionActive;
	
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	
	UFUNCTION(BlueprintCallable)
	int& GetIntensityMeter(){return IntensityMeter;};
	UFUNCTION(BlueprintCallable)
	void LowerIntensityMeter(int Amount);
	UFUNCTION(BlueprintCallable)
	void IncreaseIntensityMeter(int Amount);

	
	
};
