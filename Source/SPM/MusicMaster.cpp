// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicMaster.h"
#include "Kismet/GameplayStatics.h"


void UMusicMaster::IncreaseIntensityMeter(int Amount)
{
	//Check What Intensity The Meter is going to be
	const int NewIntensity = IntensityMeter + Amount;
	if(NewIntensity >= MAXINTENSITY)
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

	if(NewIntensity <= 0)
	{
		return;
	}
	IntensityMeter -= Amount;
	if(NewIntensity < MAXINTENSITY)
	{
		bIsTransitionActive = true;
		ResetTimer();
		Caller.Clear();
		Caller.BindDynamic(this, &UMusicMaster::CallMed);
		return;
	}
	if(NewIntensity < MEDIUMINTENSITY)
	{
		bIsTransitionActive = true;
		ResetTimer();
		Caller.Clear();
		Caller.BindDynamic(this, &UMusicMaster::CallLow);
		return;
	}

	/*
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

void UMusicMaster::ResetTimer()
{
	Timer = 10;
}

void UMusicMaster::StartTimer()
{
	
	if(Timer <= 0)
	{
		Timer = 10;
		Caller.Execute();
		bIsTransitionActive = false;
		//Check What Music Should be transitioned To with the Delegates
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("%f"), Timer);
	Timer -= UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	
}

void UMusicMaster::Tick(float DeltaTime)
{
	if(bIsTransitionActive)
	{
		StartTimer();
	}
}



bool UMusicMaster::IsTickableInEditor() const
{
	return false;
}

bool UMusicMaster::IsTickableWhenPaused() const
{
	return false;
}


bool UMusicMaster::IsTickable() const
{
	return true;
}

TStatId UMusicMaster::GetStatId() const
{
	return TStatId();
}

