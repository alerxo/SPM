// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicMaster.h"
#include "Kismet/GameplayStatics.h"


void UMusicMaster::IncreaseIntensityMeter(int Amount)
{
	
	const ULink* UpperLink = CurrentIntensityNode->UpperLink;
	const ULink* DownLink = CurrentIntensityNode->DownLink;
	UE_LOG(LogTemp, Warning, TEXT("Increase: INTENSITY METER: %i"), Amount)
	if(Amount >= (UpperLink ? UpperLink->Limit : Amount+1)) 
	{
		UpperLink->Link->Caller.Execute();
		CurrentIntensityNode->bIsOn = false;
		bIsTransitionActive = true;
		ResetTimer();
		CurrentIntensityNode = UpperLink->Link;
		return;
	}
	ResetTimer();
	/*
	if(Amount <= (DownLink ? DownLink->Limit : -1))
	{
		DownLink->Link->Caller.Execute();
		CurrentIntensityNode->bIsOn = false;
		CurrentIntensityNode = DownLink->Link;
		return;
	}
	*/

	
	//Check What Intensity The Meter is going to be
	/*
	const int NewIntensity = IntensityMeter + Amount;
	if(NewIntensity >= MAXINTENSITY)
	{
		return;
	}

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
	*/
}

void UMusicMaster::LowerIntensityMeter(int Amount)
{
	/*
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
	Timer = RESETTIMER;
}

void UMusicMaster::StartTimer()
{
	
	if(Timer <= 0)
	{
		Timer = RESETTIMER;
		if(CurrentIntensityNode && CurrentIntensityNode->DownLink)
		{
			CurrentIntensityNode->DownLink->Link->Caller.Execute();
			CurrentIntensityNode = CurrentIntensityNode->DownLink->Link;
			bIsTransitionActive = CurrentIntensityNode->DownLink ? true : false;
		}
		//Check What Music Should be transitioned To with the Delegates
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("%f"), Timer);
	Timer -= UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	
}

void UMusicMaster::Tick(float DeltaTime)
{

	/*
	if(CurrentIntensityNode)
	{
		CurrentIntensityNode->Caller.Execute();
	}
	*/
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

void  UMusicMaster::CallMax()
{
	CurrentIntensityNode->bIsOn = true;
	OnMaxIntensity.Broadcast();
}

void  UMusicMaster::CallMed()
{
	CurrentIntensityNode->bIsOn = true;
	OnMediumIntensity.Broadcast();
}
void  UMusicMaster::CallLow()
{
	CurrentIntensityNode->bIsOn = true;
	OnLowIntensity.Broadcast();
}

void UMusicMaster::SetUp()
{
	Low = NewObject<UIntensityNode>();
	Low->UpperLink = NewObject<ULink>();
	Medium = NewObject<UIntensityNode>();
	Medium->UpperLink = NewObject<ULink>();
	Medium->DownLink = NewObject<ULink>();
	High = NewObject<UIntensityNode>();
	High->DownLink = NewObject<ULink>();
	
	Low->UpperLink->Limit = MEDIUMINTENSITY;
	Low->UpperLink->Link = Medium;
	Low->Caller.BindDynamic(this, &UMusicMaster::CallLow);
	
	Medium->DownLink->Limit = MEDIUMINTENSITY;
	Medium->DownLink->Link = Low;
	Medium->UpperLink->Link = High;
	Medium->UpperLink->Limit = MAXINTENSITY;
	Medium->Caller.BindDynamic(this, &UMusicMaster::CallMed);

	High->DownLink->Limit = MAXINTENSITY;
	High->DownLink->Link = Medium;
	High->Caller.BindDynamic(this, &UMusicMaster::CallMax);

	CurrentIntensityNode = Low;
	CurrentIntensityNode->bIsOn = false;
}
