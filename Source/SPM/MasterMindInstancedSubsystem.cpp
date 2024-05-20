// Fill out your copyright notice in the Description page of Project Settings.

//Give interface an EnemyType
//Each Enemy Give it self the Enum it is in
#include "MasterMindInstancedSubsystem.h"

#include "EnemyInfo.h"
#include "EnemyInterface.h"
#include "Spawner.h"


//Initialize SubSystem
void UMasterMindInstancedSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	/*
	SpiderEnum = UEnemiesEnum::Instiantiate(ESpider);
	DroneEnum = UEnemiesEnum::Instiantiate(EDrone);
	WallBreakerEnum = UEnemiesEnum::Instiantiate(EWallbreaker);
	*/
	AllEnemyStats.Init(FEnemyStats(0,0,0,0,0), 3);
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

void UMasterMindInstancedSubsystem::IncreaseWeight(TEnumAsByte<EEnemies> Enemy, double Amount)
{
	if(!AllEnemyStats.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Before Weight Increase %f"),AllEnemyStats[Enemy].Weight);
		AllEnemyStats[Enemy.GetIntValue()].Weight += Amount;
		TotalEnemyWeight+= Amount;
		UE_LOG(LogTemp, Warning, TEXT("Enemy After Weight Increase %f"), AllEnemyStats[Enemy].Weight);
	}

}
void UMasterMindInstancedSubsystem::IncreaseDamageAmount(TEnumAsByte<EEnemies> Enemy, float Amount)
{
	if(!AllEnemyStats.IsEmpty())
	{
		AllEnemyStats[Enemy.GetIntValue()].DamageAmount += Amount;
		UE_LOG(LogTemp, Warning, TEXT("Enemy Damage Amount %f"),AllEnemyStats[Enemy.GetIntValue()].DamageAmount);
	}
}

void UMasterMindInstancedSubsystem::IncreaseHit(TEnumAsByte<EEnemies> Enemy)
{
	if(!AllEnemyStats.IsEmpty())
	{
		AllEnemyStats[Enemy.GetIntValue()].TotalHits++;
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
void UMasterMindInstancedSubsystem::UpdateWeight(double Amount)
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
		AllEnemyStats[Enemy.GetIntValue()].Amount++;
		TotalEnemyAmount++;
		UE_LOG(LogTemp, Warning, TEXT("Increase Enemy Amount: %i"), AllEnemyStats[0].Amount);
	}
}

void UMasterMindInstancedSubsystem::IncreaseEnemyKilled(TEnumAsByte<EEnemies> Enemy)
{
	if(!AllEnemyStats.IsEmpty())
	{
		AllEnemyStats[Enemy.GetIntValue()].Killed++;
	}
}

double UMasterMindInstancedSubsystem::BalanceKilledAndDamage(TEnumAsByte<EEnemies> Enemy)
{
	//Get the Different Values
	double KillPerAmount = 0; 
	if(!AllEnemyStats.IsEmpty())
	{
		double& Weight = AllEnemyStats[Enemy.GetIntValue()].Weight;
		int& KilledVal = AllEnemyStats[Enemy.GetIntValue()].Killed;
		int& Amount = AllEnemyStats[Enemy.GetIntValue()].Amount;
		float& DamageVal = AllEnemyStats[Enemy.GetIntValue()].DamageAmount;

		if(Amount > 0)
		{
			KillPerAmount =(double)KilledVal/ (double)Amount;
		}
		
		UE_LOG(LogTemp, Warning , TEXT("Killed = %i"), KilledVal)
		UE_LOG(LogTemp, Warning , TEXT("Amount = %i"), Amount)
	}

	return KillPerAmount;
}

void UMasterMindInstancedSubsystem::IncreasaEnemyAmount(FEnemyStats& EnemyStats)
{
	EnemyStats.Amount++;
}

FEnemyStats UMasterMindInstancedSubsystem::CreateEnemyStats(double Weight)
{
	FEnemyStats EnemyStats = FEnemyStats(0,0,0,Weight,0);
	return EnemyStats;
}


double UMasterMindInstancedSubsystem::WeightProcentageOfEnemy(TEnumAsByte<EEnemies> Enemy)
{
	if(!AllEnemyStats.IsEmpty())
	{
		return (AllEnemyStats[Enemy.GetIntValue()].Weight)/TotalEnemyWeight;	
	}
	UE_LOG(LogTemp, Error, TEXT("The list is Empty UMasterMindInstancedSubsystem::WeightProcentageOfEnemy"))
	return 0.0;
}

void UMasterMindInstancedSubsystem::IncreaseDecisionMeter(int Amount)
{
	DecisionMeter += Amount;
}

void UMasterMindInstancedSubsystem::ResetDecisionMeter()
{
	DecisionMeter = 0;
}


