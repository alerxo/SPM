// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicMaster.h"


void UMusicMaster::IncreaseIntensityMeter(int Amount)
{
	//Check What Intensity The Meter is going to be
	const int NewIntensity = IntensityMeter + Amount;
	if(NewIntensity >= 100)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Increase: INTENSITY METER: %i"), NewIntensity)
	if (NewIntensity >= MAXINTENSITY && CurrentIntensity != HighIntensity)
	{
		CurrentIntensity = HighIntensity;
		
		IntensityMeter += Amount;
		OnMaxIntensity.Broadcast();
		return;
	}
	if(NewIntensity >= MEDIUMINTENSITY)
	{
		OnMediumIntensity.Broadcast();
		CurrentIntensity = MediumIntensity;
		IntensityMeter += Amount;
		return;
	}
	if(NewIntensity >= LOWINTENSITY)
	{
		OnLowIntensity.Broadcast();
		CurrentIntensity = LowIntensity;
		IntensityMeter += Amount;
		return;
	}
	IntensityMeter += Amount;
}

void UMusicMaster::LowerIntensityMeter(int Amount)
{
	const int NewIntensity = IntensityMeter - Amount;
	UE_LOG(LogTemp, Warning, TEXT("Decrease: INTENSITY METER: %i"), NewIntensity)
	if(NewIntensity <= 0)
	{
		IntensityMeter = 0;
	}
	else
	{
		IntensityMeter = NewIntensity;
	}
	
	if(IntensityMeter < HighIntensity && CurrentIntensity == HighIntensity)
	{
		OnMediumIntensity.Broadcast();
		return;
	}
	if(IntensityMeter < MEDIUMINTENSITY && CurrentIntensity == MediumIntensity)
	{
		OnLowIntensity.Broadcast();
		return;
	}
	/*
	if(NewIntensity <= LOWINTENSITY && !(CurrentIntensity == LowIntensity))
	{
		IntensityMeter -= Amount;
	}
	*/
}
