// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

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
	// ...
	CurrentObjPoolPosition = 0; 
}


// Called every frame
void USpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int USpawner::SpawnAtLocation(int TotalTokens)
{


	if(Time <= 0)
	{
		int rand = FMath::RandRange(0, SpawnLocations.Max() -1);
		if(EnemyObjectPool.IsValidIndex(0))
		{
			FVector Pos = (SpawnLocations[rand]->GetActorForwardVector() * YOffset) +  SpawnLocations[rand]->GetActorLocation();
			EnemyObjectPool[0]->SetActorLocation(Pos);
			EnemyObjectPool.RemoveAt(0);
			TotalTokens -= 10;

		}
		
		Time = DefaultTime;
		return TotalTokens;
	}
	UE_LOG(LogTemp, Warning ,TEXT("%f"), Time);
	Time-= 1 * GetWorld()->DeltaTimeSeconds;

	return  TotalTokens;
}


