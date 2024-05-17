// Fill out your copyright notice in the Description page of Project Settings.

//Give interface an EnemyType
//Each Enemy Give it self the Enum it is in
#include "MasterMindInstancedSubsystem.h"
#include "EnemyInterface.h"

//Initialize SubSystem
void UMasterMindInstancedSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	/*
	SpiderEnum = UEnemiesEnum::Instiantiate(ESpider);
	DroneEnum = UEnemiesEnum::Instiantiate(EDrone);
	WallBreakerEnum = UEnemiesEnum::Instiantiate(EWallbreaker);
	*/
}

void UMasterMindInstancedSubsystem::SetUp()
{
	/*
	SpiderEnum = NewObject<UEnemiesEnum>();
	SpiderEnum->Enemy = ESpider;
	SpiderEnum->MostTotalHits = 0;
	ListOfAllEnemieEnum.AddUnique(SpiderEnum);
	*/
}

//Deinitialize SubSystem
void UMasterMindInstancedSubsystem::Deinitialize()
{
	Super::Deinitialize();
	/*
	delete SpiderEnum;
	delete DroneEnum;
	delete WallBreakerEnum;
	*/
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

/*
TArray<FEnemyStats> UMasterMindInstancedSubsystem::GetArrayOfStats()
{
	return TArray<FEnemyStats>{SpiderStats, DroneStats, WallBreakerStats};
}
*/
void UMasterMindInstancedSubsystem::Hit(UEnemiesEnum* Enemy)
{
	Enemy->MostTotalHits++; 
}

//Get the Enemy Type of the Enemy
TEnumAsByte<EEnemies> UMasterMindInstancedSubsystem::GetEnemyType( TScriptInterface<IEnemyInterface> GeneralEnemy)
{
	return GeneralEnemy.GetInterface()->EnemyType;
}

void UMasterMindInstancedSubsystem::IncreaseWeight(TEnumAsByte<EEnemies> Enemy)
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Before Weight Increase %i"),AllEnemyStats[Enemy].Weight);
	AllEnemyStats[Enemy].Weight++;
	UE_LOG(LogTemp, Warning, TEXT("Enemy After Weight Increase %i"), AllEnemyStats[Enemy].Weight);
}
void UMasterMindInstancedSubsystem::IncreaseDamageAmount(TEnumAsByte<EEnemies> Enemy, float Amount)
{
	AllEnemyStats[Enemy].DamageAmount += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Enemy Damage Amount %f"),AllEnemyStats[Enemy].DamageAmount);
}




