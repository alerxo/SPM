// Fill out your copyright notice in the Description page of Project Settings.


#include "SPMGameInstanceSubsystem.h"

//Initialize SubSystem
void USPMGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

//Deinitialize SubSystem
void USPMGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

//Getter for USPMGameInstanceSubsystem 
USPMGameInstanceSubsystem* USPMGameInstanceSubsystem::GetSPMGameInstanceSubSystem()
{
	return this;
}


