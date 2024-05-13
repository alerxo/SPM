// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "AIController.h"
#include "SpawnPoints.h"
#include "Spiderbot.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USpawner::USpawner()
{

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawner::BeginPlay()
{
	Super::BeginPlay();
	
	
	Time = DefaultTime;
	CurrentObjPoolPosition = 0; 
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


