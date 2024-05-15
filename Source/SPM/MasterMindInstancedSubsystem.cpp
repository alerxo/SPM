// Fill out your copyright notice in the Description page of Project Settings.

//Give interface an EnemyType
//Each Enemy Give it self the Enum it is in
#include "MasterMindInstancedSubsystem.h"
#include "EnemyInterface.h"

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

bool UMasterMindInstancedSubsystem::RequestToken(APawn* Pawn)
{
	
	if(Tokens > 0)
	{
		Tokens--;
		return  true;
	}
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

FVector UMasterMindInstancedSubsystem::GetInvestigationLocation() const 
{
	return  InvestigationLocation;
}

void UMasterMindInstancedSubsystem::SetInvestigationLocation(FVector Vector)
{
	InvestigationLocation = Vector;

	
}

TArray<FEnemyStats> UMasterMindInstancedSubsystem::GetArrayOfStats()
{
	return TArray<FEnemyStats>{SpiderStats, DroneStats, WallBreakerStats};
}

void UMasterMindInstancedSubsystem::Hit(UEnemiesEnum* Enemy)
{
	Enemy->MostTotalHits++; 
}

EEnemies UMasterMindInstancedSubsystem::GetEnemyType(IEnemyInterface* GeneralEnemy)
{
	return GeneralEnemy->EnemyType;
}





