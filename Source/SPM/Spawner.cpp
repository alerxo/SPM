// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "AIController.h"
#include "EnemyInfo.h"
#include "EnemyObjectPool.h"
#include "SpawnPoints.h"
#include "Spiderbot.h"
#include "BehaviorTree/BehaviorTree.h"
#include "MasterMindInstancedSubsystem.h"
#include "Engine/LevelScriptActor.h"
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
	//Pool = new EnemyObjectPool();


	//ListRandom = NewObject<URandomList>();
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
	Time = DefaultTime;
}


// Called every frame
void USpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
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

	float Deviation = 300;
	float BestDifference = NAN;

	if(BehaviourTree == nullptr)
	{
		return CurrentBest;
	}

	//Params for the spawning
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Owner;
	SpawnParameters.SpawnCollisionHandlingOverride = false ? ESpawnActorCollisionHandlingMethod::AlwaysSpawn : ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;


	int i = 0;
	//Decide the best Spawn position
	for (ASpawnPoints* SpawnPoint : SpawnLocations)
	{
		i++;
		float Dist = FVector::Dist(PlayerPos, SpawnPoint->GetActorLocation());
		bool const MaxRangeCheck = (Dist <= MaxRange) ? true : ((Dist -=Deviation) <= MaxRange);
		bool const MinRangeCheck = (Dist >= Range) ? true : ((Dist+=Deviation) >= Range );
		if(MaxRangeCheck && MinRangeCheck)
		{
			//Create Enemy And set the Ai behaviour on it
			FVector Forward = SpawnPoint->GetActorForwardVector();
			Forward.X = +Forward.X;
			FVector const Location = SpawnPoint->GetActorLocation() + (YOffset * Forward);
			APawn* Enemy = GetWorld()->SpawnActor<APawn>(ActorToSpawn, Location, Rotator, SpawnParameters);
			const IEnemyInterface* CastedEnemy = Cast<IEnemyInterface>(Enemy);
			
			UE_LOG(LogTemp, Display, TEXT("---------------- ENEMY AMOUNT  %i -----------------------------") ,MasterMind->TotalEnemyAmount) 
			MasterMind->AllSpawnedEnemies.Add(MasterMind->CurrentMapIndex++, Enemy);
			
			if(Enemy != nullptr)
			{
				SpawnAI(Enemy, BehaviourTree);
			}
			
			return SpawnPoint;
		}

		//Check the Spawn point if it has a better difference than other last best
		const float& MaxDifference = FMath::Abs(MaxRange - Dist);
		const float& MinDifference = FMath::Abs(Dist - Range);
		MaxDifference < BestDifference ? BestDifference = MaxDifference, CurrentBest = SpawnPoint : nullptr;
		MinDifference < BestDifference ? BestDifference = MinDifference, CurrentBest = SpawnPoint : nullptr;
	}
	
	if(CurrentBest)
	{
		//Create Enemy And set the Ai behaviour on it
		FVector const Location = CurrentBest->GetActorLocation() + ( YOffset * CurrentBest->GetActorForwardVector());
		APawn* Enemy = GetWorld()->SpawnActor<APawn>(ActorToSpawn, Location, Rotator, SpawnParameters);
		UE_LOG(LogTemp, Display, TEXT("---------------- ENEMY AMOUNT  %i -----------------------------") ,MasterMind->TotalEnemyAmount)
		MasterMind->AllSpawnedEnemies.Add(MasterMind->CurrentMapIndex++, Enemy);

		MasterMind->MoveActor(Enemy);
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
 
UBehaviorTree* USpawner::RandomWithWeight(FEnemyStats& Enemy, bool OverrideChance, FEnemyStats OverrideEnemy)
{
	float const Weight = MasterMind->TotalEnemyWeight;
	if(OverrideChance)
	{
		Enemy = OverrideEnemy;
		return Enemy.EnemyTree;
	}
	float Num = FMath::FRandRange(1.0, Weight);
	for(FEnemyStats Type : EnemyList)
	{
		FEnemyStats& EnemyStats = MasterMind->AllEnemyStats[Type.EnemyType.GetIntValue()];
		if(EnemyStats.Weight <= 0)
		{
			continue;
		}
		
		int index = Type.EnemyType.GetValue();
		Num -= EnemyStats.Weight;
		if(Num <= 0)
		{
			Enemy = Type;
			return Type.EnemyTree;
		}
		
	}
	if(!MasterMind->AllEnemyStats.IsEmpty())
	{
		Enemy =  MasterMind->AllEnemyStats[0];
		return Enemy.EnemyTree;
	}
	return nullptr;
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
		MasterMind->AllSpawnedEnemies.Add(MasterMind->CurrentMapIndex++, Enemy);
	}
}

