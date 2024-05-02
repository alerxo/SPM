// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterMindInstancedSubsystem.h"

//Initialize SubSystem
void UMasterMindInstancedSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

//Deinitialize SubSystem
void UMasterMindInstancedSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

//Getter for USPMGameInstanceSubsystem 
UMasterMindInstancedSubsystem* UMasterMindInstancedSubsystem::GetMasterMindInstancedSubsystem()
{
	return this;
}

bool UMasterMindInstancedSubsystem::RequestToken()
{
	if(Tokens > 0)
	{
		Tokens--;
		return  true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Tokens Have Run Out"));
	return false;
}

float UMasterMindInstancedSubsystem::GetTokens()
{
	return Tokens;
}

void UMasterMindInstancedSubsystem::SetTokens(float Amount)
{
	Tokens = Amount;
}



