// Fill out your copyright notice in the Description page of Project Settings.

//Give interface an EnemyType
//Each Enemy Give it self the Enum it is in
#include "MasterMindInstancedSubsystem.h"

#include "EnemyInfo.h"
#include "EnemieEnum.h"
#include "EnemyInterface.h"

#include "Spawner.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"


//Initialize SubSystem
void UMasterMindInstancedSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	/*
	SpiderEnum = UEnemiesEnum::Instiantiate(ESpider);
	DroneEnum = UEnemiesEnum::Instiantiate(EDrone);
	WallBreakerEnum = UEnemiesEnum::Instiantiate(EWallbreaker);
	*/
	AllEnemyStats.Init(FEnemyStats(0,0,0,0,0), UEnemiesEnum::Size);
}
void UMasterMindInstancedSubsystem::SetPlayer()
{
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
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

	if(Player != NULL)
	{
		const IEnemyInterface* CastedEnemy = Cast<IEnemyInterface>(Pawn);
		const int& EnemyTokens = AllEnemyStats[CastedEnemy->EnemyType.GetIntValue()].TokenCost;
		if(Tokens - EnemyTokens >= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("TOKEN:  %i"), Tokens)
			Tokens -= EnemyTokens;
		
			//DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(),175, 6, FColor::Green,false, 1);
			MapOfTokens.Add(Pawn, EnemyTokens);
			return true;
		}
		/*
		float Dot = FVector::DotProduct(Pawn->GetActorForwardVector() ,Player->GetActorForwardVector());
		Dot += 0.7;
		if(Dot < 0)
		{
			DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(),175, 6, FColor::Orange,false, 1);
			//Steal Token From Enemy
			PriorityEnemies.
			return true;
		}
		*/
	}
	return false;
}

void UMasterMindInstancedSubsystem::AddTokensLimit(float MultiplyProcentage)
{
	if(MultiplyProcentage <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("MultiplyProcentage is under or equal to zero, UMasterMindInstancedSubsystem::UpdateTokensLimit"))
	}
	const int NewTokenAmount = (float)DefaultTokens * MultiplyProcentage;
	Tokens += NewTokenAmount - Tokens;
	UE_LOG(LogTemp, Warning, TEXT("ADD Tokens:   %i :: NEWTOKENAMOUNT:  %i"), Tokens, NewTokenAmount);
}

void UMasterMindInstancedSubsystem::ReduceTokenLimit(float MultiplyProcentage)
{
	if(MultiplyProcentage <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("MultiplyProcentage is under or equal to zero, UMasterMindInstancedSubsystem::UpdateTokensLimit"))
	}
	MapOfTokens.Reset();
	const int NewTokenAmount = (float)DefaultTokens * MultiplyProcentage;
	Tokens = NewTokenAmount;
	UE_LOG(LogTemp, Warning, TEXT("REDUCE Tokens: %i :: NEWTOKENAMOUNT: %i"), Tokens, NewTokenAmount);
}



void UMasterMindInstancedSubsystem::HandBackToken(int Amount, APawn* Pawn)
{
	Tokens += Amount;
	MapOfTokens.Remove(Pawn);
}

void UMasterMindInstancedSubsystem::CheckAndDeleteToken(TEnumAsByte<EEnemies> EnemyType, APawn* Pawn)
{
	if(Pawn != nullptr && MapOfTokens.Find(Pawn))
	{
		
		const int& TokenAmount = AllEnemyStats[EnemyType.GetIntValue()].TokenCost;
		if(TokenAmount < 0)
		{
			UE_LOG(LogTemp, Error, TEXT("The Token Cost is Zero or Belove, UMasterMindInstancedSubsystem::CheckAndDeleteToken"));
		}
		UE_LOG(LogTemp, Warning, TEXT("Should Add to Token"))
		Tokens += TokenAmount;
		UE_LOG(LogTemp, Warning, TEXT("TOKEN Amount:  %i"), TokenAmount)
		MapOfTokens.Remove(Pawn);
	}
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

void UMasterMindInstancedSubsystem::ResetWeight()
{
	TotalEnemyWeight = 0;
}


void UMasterMindInstancedSubsystem::IncreasEnemyAmount(TEnumAsByte<EEnemies> Enemy)
{
	if(!AllEnemyStats.IsEmpty())
	{
		AllEnemyStats[Enemy.GetIntValue()].Amount++;
		TotalEnemyAmount++;
		UE_LOG(LogTemp, Warning, TEXT("Increase %s Amount: %i "), *UEnum::GetValueAsName(Enemy).ToString() ,AllEnemyStats[Enemy.GetIntValue()].Amount);
	}
}

void UMasterMindInstancedSubsystem::DecreaseEnemyAmount(TEnumAsByte<EEnemies> Enemy)
{
	if(!AllEnemyStats.IsEmpty())
	{
		AllEnemyStats[Enemy.GetIntValue()].Amount--;
		TotalEnemyAmount--;
		UE_LOG(LogTemp, Warning, TEXT("Decrease %s Amount: %i "), *UEnum::GetValueAsName(Enemy).ToString() ,AllEnemyStats[Enemy.GetIntValue()].Amount);
	}
}


void UMasterMindInstancedSubsystem::IncreaseEnemyKilled(TEnumAsByte<EEnemies> Enemy)
{
	if(!AllEnemyStats.IsEmpty())
	{
		AllEnemyStats[Enemy.GetIntValue()].Killed++;
		TotalEnemyAmount--;
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
			KillPerAmount = (double)KilledVal/ (double)Amount;
		}
		
		UE_LOG(LogTemp, Warning , TEXT("Killed = %i"), KilledVal)
		UE_LOG(LogTemp, Warning , TEXT("Amount = %i"), Amount)
	}

	return KillPerAmount;
}



void UMasterMindInstancedSubsystem::CreateEnemyStats(FEnemyStats EnemyStats)
{
	if(AllEnemyStats.IsValidIndex(EnemyStats.EnemyType.GetIntValue()))
	{
		int index = EnemyStats.EnemyType.GetIntValue();
		FEnemyStats& Enemy = AllEnemyStats[index];
		Enemy.Weight = EnemyStats.Weight;
		Enemy.EnemyType = EnemyStats.EnemyType;
		Enemy.TokenCost = EnemyStats.TokenCost;
		Enemy.DamageAmount = 0;
		Enemy.Killed = 0;
		UpdateWeight(Enemy.Weight);
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("UMasterMindInstancedSubsystem::CreateEnemyStats Index is not Valid"))
}


double UMasterMindInstancedSubsystem::WeightProcentageOfEnemy(TEnumAsByte<EEnemies> Enemy)
{
	if(!AllEnemyStats.IsEmpty())
	{
		return (AllEnemyStats[Enemy.GetIntValue()].Weight)/TotalEnemyWeight;	
	}UE_LOG(LogTemp, Error, TEXT("The list is Empty UMasterMindInstancedSubsystem::WeightProcentageOfEnemy"))
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

void UMasterMindInstancedSubsystem::DecreaseTokens(int Amount)
{
	if(!(Amount > Tokens))
	{
		Tokens -= Amount;
	}
	UE_LOG(LogTemp, Error, TEXT("OverSpending on Tokens UMasterMindInstancedSubsystem::DecreaseTokens"))
}

void UMasterMindInstancedSubsystem::IncreaseTokens(int Amount)
{
	Tokens += Amount;
}


UMusicMaster* UMasterMindInstancedSubsystem::CreateMusicMaster()
{
	MusicMaster = NewObject<UMusicMaster>();
	if(MusicMaster)
	{
		return MusicMaster;
	}

	UE_LOG(LogTemp,Warning,TEXT("Music Master is NULL, UMasterMindInstancedSubsystem::CreateMusicMaster"))
	return nullptr;
}


