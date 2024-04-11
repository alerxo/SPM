// Fill out your copyright notice in the Description page of Project Settings.


#include "SPMGameInstanceSubsystem.h"

void USPMGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USPMGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

USPMGameInstanceSubsystem* USPMGameInstanceSubsystem::GetSPMGameInstanceSubSystem()
{
	return this;
}


