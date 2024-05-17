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
	FEnemyStats stat = FEnemyStats(0,0,0,0,0);
	AllEnemyStats.Init(stat, 3);
	
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

void UMasterMindInstancedSubsystem::IncreaseWeight(TEnumAsByte<EEnemies> Enemy, int Amount)
{
	if(!AllEnemyStats.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Before Weight Increase %i"),AllEnemyStats[Enemy].Weight);
		AllEnemyStats[Enemy].Weight += Amount;
		TotalEnemyWeight+= Amount;
		UE_LOG(LogTemp, Warning, TEXT("Enemy After Weight Increase %i"), AllEnemyStats[Enemy].Weight);
	}

}
void UMasterMindInstancedSubsystem::IncreaseDamageAmount(TEnumAsByte<EEnemies> Enemy, float Amount)
{
	if(!AllEnemyStats.IsEmpty())
	{
		AllEnemyStats[Enemy].DamageAmount += Amount;
		UE_LOG(LogTemp, Warning, TEXT("Enemy Damage Amount %f"),AllEnemyStats[Enemy].DamageAmount);
	}
}

void UMasterMindInstancedSubsystem::IncreaseHit(TEnumAsByte<EEnemies> Enemy)
{
	if(!AllEnemyStats.IsEmpty())
	{
		AllEnemyStats[Enemy].TotalHits++;
	}
}

void UMasterMindInstancedSubsystem::UpdateWeightAllAtOnce()
{
	if(!AllEnemyStats.IsEmpty())
	{
		for (FEnemyStats EnemyStat : AllEnemyStats)
		{
			TotalEnemyWeight += EnemyStat.Weight;
		}
	}

}
void UMasterMindInstancedSubsystem::UpdateWeight(int Amount)
{
	if(!AllEnemyStats.IsEmpty())
	{
		TotalEnemyWeight += Amount;
	}

}

void UMasterMindInstancedSubsystem::IncreasEnemyAmount(TEnumAsByte<EEnemies> Enemy)
{
	if(!AllEnemyStats.IsEmpty())
	{
		AllEnemyStats[Enemy].Amount++;
		TotalEnemyAmount++;
	}
}

void UMasterMindInstancedSubsystem::IncreaseEnemyKilled(TEnumAsByte<EEnemies> Enemy)
{
	if(!AllEnemyStats.IsEmpty())
	{
		AllEnemyStats[Enemy].Killed++;
	}
}

double UMasterMindInstancedSubsystem::BalanceKilledAndDamage(TEnumAsByte<EEnemies> Enemy)
{
	//Get the Different Values
	double KillPerAmount = 0; 
	if(!AllEnemyStats.IsEmpty())
	{
		int& Weight = AllEnemyStats[Enemy].Weight;
		int& KilledVal = AllEnemyStats[Enemy].Killed;
		int& Amount = AllEnemyStats[Enemy].Amount;
		float& DamageVal = AllEnemyStats[Enemy].DamageAmount;

		if(Amount > 0)
		{
			KillPerAmount =(double)Amount/(double)KilledVal;
		}
		
		UE_LOG(LogTemp, Warning , TEXT("Killed = %i"), KilledVal)
		UE_LOG(LogTemp, Warning , TEXT("Amount = %i"), Amount)
	}

	return KillPerAmount;
}





