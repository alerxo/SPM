// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "AIController.h"
#include "EnemyInfo.h"
#include "EnemyObjectPool.h"
#include "SpawnPoints.h"
#include "Spiderbot.h"
#include "BehaviorTree/BehaviorTree.h"
#include "MasterMindInstancedSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"



// Sets default values for this component's properties
USpawner::USpawner()
{

	//Setup all the Enemies

	/*
	SpiderWeight.EnemyType = ListOfAllEnemieEnum[ESpider];
	DroneWeight.EnemyType = ListOfAllEnemieEnum[EDrone];
	WallBreakerWeight.EnemyType = ListOfAllEnemieEnum[EWallbreaker];
	*/
	
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Pool = new EnemyObjectPool();


	ListRandom = NewObject<URandomList>();
	//Set Spawn Chance on Construktion
	/*
	TArray<float> Chance{SpiderSpawnChance, DroneSpawnChance, WallbreakerSpawnChance};
	TArray<UEnemiesEnum*>Enemies={SpiderEnum, DroneEnum, WallBreakerEnum};
	ChangeSpawnChance(Chance, Enemies);
	*/
}


// Called when the game starts
void USpawner::BeginPlay()
{
	Super::BeginPlay();
	MasterMind = GetWorld()->GetGameInstance()->GetSubsystem<UMasterMindInstancedSubsystem>();

	//Add all enemies to weight list
	//WeightList={SpiderWeight, DroneWeight, WallBreakerWeight};
	
	Time = DefaultTime;
	CurrentObjPoolPosition = 0;
	/*
	TArray<float> Chance{SpiderSpawnChance, DroneSpawnChance, WallbreakerSpawnChance};
	TArray<UEnemiesEnum*>Enemies = {SpiderEnum,DroneEnum, WallBreakerEnum};
	ChangeSpawnChance(Chance, Enemies);
	*/

	/*
	SpiderWeight.EnemyEnum = ESpider;
	DroneWeight.EnemyEnum = EDrone;
	WallBreakerWeight.EnemyEnum = EWallbreaker;
	*/
}


// Called every frame
void USpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	


	// ...
}

//Method for deciding the best Spawn Position
ASpawnPoints* USpawner::BestSpawnByRange(float Range, float MaxRange, TSubclassOf<AActor> ActorToSpawn, UBehaviorTree* BehaviourTree, AActor* Owner)
{
	//PlayerPos to measure Distance between Spawn point
	const FVector PlayerPos = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	//if the best solution not found return the best of all Spawn points
	ASpawnPoints* CurrentBest = SpawnLocations[0];
	//last distance
	float LastDist = FVector::Dist(PlayerPos,SpawnLocations[0]->GetActorLocation());
	//Create Rotator 
	FRotator const Rotator = FRotator::ZeroRotator;

	//Params for the spawning
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Owner;
	SpawnParameters.SpawnCollisionHandlingOverride = false ? ESpawnActorCollisionHandlingMethod::AlwaysSpawn : ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	//Decide the best Spawn position
	for (ASpawnPoints* SpawnPoint : SpawnLocations)
	{
		const float Dist = FVector::Dist(PlayerPos, SpawnPoint->GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("Dist: %f"), Dist)
		if(Dist <= MaxRange && Dist >= Range)
		{
			//Create Enemy And set the Ai behaviour on it
			UE_LOG(LogTemp, Warning, TEXT("Direct Spawn"))
			FVector Forward = SpawnPoint->GetActorForwardVector();
			Forward.X = +Forward.X;
			FVector const Location = SpawnPoint->GetActorLocation() + (YOffset * Forward);
			APawn* Enemy = GetWorld()->SpawnActor<APawn>(ActorToSpawn, Location, Rotator, SpawnParameters);
			//Set a AI controller and behaviour tree to the enemy
			if(Enemy != nullptr)
			{
				SpawnAI(Enemy, BehaviourTree);
			}
			
			return SpawnPoint;
		}
			
		if(Dist > LastDist)
		{
			CurrentBest = SpawnPoint;
			LastDist = Dist;
		}

	}
	
	if(CurrentBest)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Best Spawn"))
		//Create Enemy And set the Ai behaviour on it
		FVector const Location = CurrentBest->GetActorLocation() + ( YOffset * CurrentBest->GetActorForwardVector());
		APawn* Enemy = GetWorld()->SpawnActor<APawn>(ActorToSpawn, Location, Rotator, SpawnParameters);
		//Set a AI controller and behaviour tree to the enemy
		if(Enemy != nullptr)
		{
			SpawnAI(Enemy, BehaviourTree);
		}

	}

	
	return CurrentBest;
}

void USpawner::SpawnAI(APawn* Enemy, UBehaviorTree* BehaviourTree)
{
	if(Enemy != NULL && Enemy->GetController() == NULL)
	{
		Enemy->SpawnDefaultController();
	}
	if(BehaviourTree == NULL)
	{
		AAIController* Controller =  Cast<AAIController>(Enemy->Controller);
		if(Controller != NULL)
		{
			Controller->RunBehaviorTree(BehaviourTree);
		}
	}
}


int USpawner::SpawnAtLocation(int TotalTokens)
{
	return 0;
}

void USpawner::RunSpawning()
{
	for(int i = 0; i < 10; i++)
	{
		Pool->SpiderBot.Add(Spawn(SpiderBot, SpiderBT));
	}
}

APawn* USpawner::Spawn(TSubclassOf<AActor> ActorToSpawn, UBehaviorTree* BehaviourTree)
{
	//Create Rotator 
	FRotator const Rotator = FRotator::ZeroRotator;
	FVector const Location = FVector::ZeroVector;
	//Params for the spawning<
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = nullptr;
	SpawnParameters.SpawnCollisionHandlingOverride = false ? ESpawnActorCollisionHandlingMethod::AlwaysSpawn : ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	APawn* Enemy = GetWorld()->SpawnActor<APawn>(ActorToSpawn, Location, Rotator, SpawnParameters);
	//Set a AI controller and behaviour tree to the enemy
	SpawnAI(Enemy, BehaviourTree);

	return Enemy;
}

void USpawner::RunDelete()
{
	
	for(APawn* Spider: Pool->SpiderBot)
	{
		Pool->SpiderBot.Remove(Spider);
		Spider->Destroy();
	}
}


//Randomly pick and enemy from a list and return a behaviour tree 
UBehaviorTree* USpawner::RandomEnemy(TSubclassOf<APawn>& Enemy, float& Range, bool OverrideChance, UEnemiesEnum* OverrideEnemy )
{
	const int32 Index = FMath::RandRange(0, ListRandom->Length() - 1);
	TEnumAsByte<EEnemies> Chosen = ESpider;
	if(ListRandom->RandomListChance.IsValidIndex(Index))
	{
		Chosen = ListRandom->RandomListChance[Index];
	}
	else
	{
		
	}
	if(OverrideChance)
	{
		//Chosen = OverrideEnemy;	
	}
	/*
	switch(Chosen->GetValue())
	{
		case 0:
			Enemy = SpiderBot;
			Range = SpiderSpawnRange;
			MasterMind->SpiderAmount++;
		delete Chosen;
			return SpiderBT;
			break;
		case 1:
			Enemy = Drone;
			Range = DroneSpawnRange;
			MasterMind->DroneAmount++;
		delete Chosen;
			return DroneBT;
			break;
		case 2:
			Enemy = Wallbreaker;
			Range = WallbreakerSpawnRange;
			MasterMind->WallbreakerAmount++;
		delete Chosen;
			return WallbreakerBT;
			break;
	}
	UE_LOG(LogTemp, Error, TEXT("No Enemy Found in Spawner::RandomEnemy"))
	*/
	return nullptr;
}

 
UBehaviorTree* USpawner::RandomWithWeight(FEnemyStats& Enemy, bool OverrideChance, FEnemyStats OverrideEnemy)
{
	float const Weight = MasterMind->TotalEnemyWeight;
	if(OverrideChance)
	{
		Enemy = OverrideEnemy;
		return Enemy.EnemyTree;
	}
	float num = FMath::FRandRange(1.0, Weight);
	for(FEnemyStats Type : ReworkList)
	{
		FEnemyStats& EnemyStats = MasterMind->AllEnemyStats[Type.EnemyType.GetValue()];
		if(EnemyStats.Weight <= 0)
		{
			continue;
		}
		
		int index = Type.EnemyType.GetValue();
		UE_LOG(LogTemp,Error, TEXT("Index: %i, Weight: %d"), index, EnemyStats.Weight)
		num -= EnemyStats.Weight;
		if(num <= 0)
		{
			Enemy = Type;
			return Type.EnemyTree;
		}
		
	}
	UE_LOG(LogTemp,Error, TEXT("Did Not Find Choose Correctly, WeightTotal: %i"), Weight)
	Enemy =  MasterMind->AllEnemyStats[0];
	return Enemy.EnemyTree;


/*
	int Weight = TotalWeight;
	if(OverrideChance)
	{
		Enemy = OverrideEnemy;
		return Enemy.BehaviorTree;
	}
	int num = FMath::RandRange(0, Weight);
	for(FEnemyWeight Type : WeightList)
	{
		FEnemyStats& EnemyStats = MasterMind->AllEnemyStats[Type.EnemyEnum.GetValue()];
		int index = Type.EnemyEnum.GetValue();
		//UEnemiesEnum* temp = ListOfAllEnemieEnum[index];
		num -= Type.Weight;
		if(num <= 0)
		{
			Enemy = Type;
			return Type.BehaviorTree;
		}
		
	}
	Enemy = SpiderWeight;
	return SpiderWeight.BehaviorTree;
	*/
}


void USpawner::SetSpawnChances()
{
	/*
	TArray<float> Chance{SpiderSpawnChance, DroneSpawnChance, WallbreakerSpawnChance};
	TArray<UEnemiesEnum*>Enemies{SpiderEnum, DroneEnum, WallBreakerEnum};
	ChangeSpawnChance(Chance, Enemies);
	*/
}


void USpawner::ChangeSpawnChance(TArray<float> Chances, TArray<UEnemiesEnum*> Enemies)
{
	
	int i = 0;
	int ChanceIndex = 0;
	for (UEnemiesEnum* Type : Enemies)
	{
		for(int j = 0; j <= static_cast<int>(Chances[ChanceIndex] * 10) - 1; j++)
		{
			if(ListRandom->RandomListChance.IsValidIndex(i))
			{
				//ListRandom->RandomListChance[i++] = Type;
			}
			
		}
		ChanceIndex++;
	}
}

void USpawner::SpawnAtSpawnPoint(ASpawnPoints* SpawnPoint, TSubclassOf<AActor> ActorToSpawn, UBehaviorTree* BehaviourTree, AActor* Owner)
{
	
	FRotator const Rotator = FRotator::ZeroRotator;

	//Params for the spawning
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Owner;
	SpawnParameters.SpawnCollisionHandlingOverride = false ? ESpawnActorCollisionHandlingMethod::AlwaysSpawn : ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	FVector Forward = SpawnPoint->GetActorForwardVector();
	Forward.X = +Forward.X;
	FVector const Location = SpawnPoint->GetActorLocation() + (YOffset * Forward);
	APawn* Enemy = GetWorld()->SpawnActor<APawn>(ActorToSpawn, Location, Rotator, SpawnParameters);
	//Set a AI controller and behaviour tree to the enemy
	if(Enemy && BehaviourTree)
	{
		SpawnAI(Enemy, BehaviourTree);
	}
}

