// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MusicMaster.generated.h"

/**
 * 
 */
//Constant Macros
#define MAXINTENSITY 5
#define MEDIUMINTENSITY 2  
#define LOWINTENSITY 0

#define RESETTIMER 5

//Enum for the different Intensities
enum IntensityValues
{
	LowIntensity,
	MediumIntensity,
	HighIntensity,
};

class UIntensityNode;

DECLARE_DYNAMIC_DELEGATE(FCaller);


//Link class that points att a Node /Representing a neighbour node
UCLASS()
class ULink : public UObject
{
	GENERATED_BODY()
public:

	//The Intensity Limit
	int Limit;
	UPROPERTY()
	UIntensityNode* Link;
};

//Node class that can hold links to other nodes.
//The node is made for calling the Intensity mode with Caller
UCLASS()
class UIntensityNode : public  UObject
{
	GENERATED_BODY()
public:
	//Upper Link that links to a Node with higher Intensity
	UPROPERTY()
	ULink* UpperLink;
	//Down link that links to a node with lower Intensity
	UPROPERTY()
	ULink* DownLink;
	
	UPROPERTY()
	FCaller Caller;

	bool bIsOn;
	
};


//Different Intensity Delegates, Called when the music should change
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMaxIntensity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMediumIntensity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLowIntensity);
UCLASS()
class SPM_API UMusicMaster : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

private:

	IntensityValues CurrentIntensity;

	UFUNCTION()
	void CallMax();
	UFUNCTION()
	void CallMed();
	UFUNCTION()
	void CallLow();

	UPROPERTY()
	UIntensityNode* Low;
	UPROPERTY()
	UIntensityNode* Medium;
	UPROPERTY()
	UIntensityNode* High;

public:

	UPROPERTY(BlueprintAssignable)
	FOnMaxIntensity OnMaxIntensity;
	UPROPERTY(BlueprintAssignable)
	FOnMediumIntensity OnMediumIntensity;
	UPROPERTY(BlueprintAssignable)
	FOnLowIntensity OnLowIntensity;

	UPROPERTY()
	UIntensityNode* CurrentIntensityNode; 

	UPROPERTY()
	FCaller Caller;
	
	float Timer = 10; 

	void ResetTimer();
	void StartTimer();

	bool bIsTransitionActive;

	//Diffrent Tick methods
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

	//Lowers The Intensity
	UFUNCTION(BlueprintCallable)
	void LowerIntensityMeter(int Amount);
	//Increases The IntensityMeter
	UFUNCTION(BlueprintCallable)
	void IncreaseIntensityMeter(int Amount);
	void SetUp();
	
	
};
