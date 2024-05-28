// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicMaster.h"
#include "Kismet/GameplayStatics.h"


void UMusicMaster::IncreaseIntensityMeter(int Amount)
{
	//Create UpperLink ptr of CurrentIntensity Node 
	const ULink* UpperLink = CurrentIntensityNode->UpperLink;
	
	if(Amount >= (UpperLink ? UpperLink->Limit : Amount+1)) 
	{
		//Update CurrentIntensityNode and execute the needed music.
		UpperLink->Link->Caller.Execute();
		CurrentIntensityNode->bIsOn = false;
		bIsTransitionActive = true;
		CurrentIntensityNode = UpperLink->Link;
	}
	ResetTimer();
}

void UMusicMaster::LowerIntensityMeter(int Amount)
{
}

void UMusicMaster::ResetTimer()
{
	Timer = RESETTIMER;
}

void UMusicMaster::StartTimer()
{
	
	if(Timer <= 0)
	{
		ResetTimer();
		if(CurrentIntensityNode && CurrentIntensityNode->DownLink)
		{
			CurrentIntensityNode->DownLink->Link->Caller.Execute();
			CurrentIntensityNode = CurrentIntensityNode->DownLink->Link;
			bIsTransitionActive = CurrentIntensityNode->DownLink ? true : false;
		}
		return;
	}
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

void  UMusicMaster::CallMax()
{
	CurrentIntensityNode->bIsOn = true;
	UE_LOG(LogTemp, Display, TEXT("MAX CALL"));
	OnMaxIntensity.Broadcast();
}

void  UMusicMaster::CallMed()
{
	CurrentIntensityNode->bIsOn = true;
	UE_LOG(LogTemp, Display, TEXT("MED CALL"));
	OnMediumIntensity.Broadcast();
}
void  UMusicMaster::CallLow()
{
	CurrentIntensityNode->bIsOn = true;
	UE_LOG(LogTemp, Display, TEXT("LOW CALL"));
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
