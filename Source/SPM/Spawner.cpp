// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "AIController.h"
#include "EnemyObjectPool.h"
#include "SpawnPoints.h"
#include "Spiderbot.h"
#include "BehaviorTree/BehaviorTree.h"
#include "MasterMindInstancedSubsystem.h"
#include "Kismet/GameplayStatics.h"



// Sets default values for this component's properties
USpawner::USpawner()
{

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Pool = new EnemyObjectPool();


	ListRandom = NewObject<URandomList>();
	//Set Spawn Chance on Construktion
	/*
	TArray<float> Chance{SpiderSpawnChance, DroneSpawnChance, WallbreakerSpawnChance};
	TArray<TEnumAsByte<EEnemies>>Enemies{ESpider, EDrone, EWallbreaker};
	ChangeSpawnChance(Chance, Enemies);
	*/
}


// Called when the game starts
void USpawner::BeginPlay()
{
	Super::BeginPlay();
	MasterMind = GetWorld()->GetGameInstance()->GetSubsystem<UMasterMindInstancedSubsystem>();
	
	Time = DefaultTime;
	CurrentObjPoolPosition = 0;
	
	for (TEnumAsByte<EEnemies> Element : ListRandom->RandomListChance)
	{
		UE_LOG(LogTemp, Warning, TEXT("ITEM: %d"), Element.GetValue());
	}
}


// Called every frame
void USpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	


	// ...
}

//Method for deciding the best Spawn Position
ASpawnPoints* USpawner::BestSpawnByRange(float Range, TSubclassOf<AActor> ActorToSpawn, UBehaviorTree* BehaviourTree, AActor* Owner)
{
	//PlayerPos to measure Distance between Spawn point
	const FVector PlayerPos = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	//if the best solution not found return the best of all Spawn points
	ASpawnPoints* CurrentBest = nullptr;
	//last distance
	float LastDist = 0;
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
		if(Range < Dist)
		{
			//Create Enemy And set the Ai behaviour on it
			FVector const Location = SpawnPoint->GetActorLocation() + ( YOffset * SpawnPoint->GetActorForwardVector());
			APawn* Enemy = GetWorld()->SpawnActor<APawn>(ActorToSpawn, Location, Rotator, SpawnParameters);
			//Set a AI controller and behaviour tree to the enemy
			SpawnAI(Enemy, BehaviourTree);
			
			return SpawnPoint;
		}
		if(Dist > LastDist)
		{
			CurrentBest = SpawnPoint;
			LastDist = Dist;
		}
	}
	//Create Enemy And set the Ai behaviour on it
	FVector const Location = CurrentBest->GetActorLocation() + ( YOffset * CurrentBest->GetActorForwardVector());
	APawn* Enemy = GetWorld()->SpawnActor<APawn>(ActorToSpawn, Location, Rotator, SpawnParameters);
	//Set a AI controller and behaviour tree to the enemy
	SpawnAI(Enemy, BehaviourTree);
	
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
UBehaviorTree* USpawner::RandomEnemy(TSubclassOf<APawn>& Enemy, float& Range)
{
	const int32 Index = FMath::RandRange(0, ListRandom->Size - 1);
	const TEnumAsByte<EEnemies> Chosen = ListRandom->RandomListChance[Index];

	UE_LOG(LogTemp, Warning, TEXT("%d"), Chosen.GetValue())
	switch (Chosen)
	{
		case 0:
			Enemy = SpiderBot;
			Range = SpiderSpawnRange;
			MasterMind->SpiderAmount++;
			return SpiderBT;
			break;
		case 1:
			Enemy = Drone;
			Range = DroneSpawnRange;
			MasterMind->DroneAmount++;
			return DroneBT;
			break;
		case 2:
			Enemy = Wallbreaker;
			Range = WallbreakerSpawnRange;
			MasterMind->WallbreakerAmount++;
			return WallbreakerBT;
			break;
	}
	UE_LOG(LogTemp, Error, TEXT("No Enemy Found in Spawner::RandomEnemy"))
	return nullptr;
}


void USpawner::SetSpawnChances()
{
	ListRandom->RandomListChance;
	TArray<float> Chance{SpiderSpawnChance, DroneSpawnChance, WallbreakerSpawnChance};
	TArray<TEnumAsByte<EEnemies>>Enemies{ESpider, EDrone, EWallbreaker};
	ChangeSpawnChance(Chance, Enemies);
}


void USpawner::ChangeSpawnChance(TArray<float> Chances, TArray<TEnumAsByte<EEnemies>> Enemies)
{
	
	int i = 0;
	int ChanceIndex = 0;
	for (TEnumAsByte<EEnemies> Type : Enemies)
	{
		for(int j = 0; j <= static_cast<int>(Chances[ChanceIndex] * 10) - 1; j++)
		{
			if(ListRandom->RandomListChance.IsValidIndex(i))
			{
				ListRandom->RandomListChance[i++] = Type.GetValue();
			}
			
		}
		ChanceIndex++;
	}
	
	
}

