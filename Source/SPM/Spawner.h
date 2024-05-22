// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemieEnum.h"
#include "MasterMindInstancedSubsystem.h"
#include "RandomList.h"
#include "Spawner.generated.h"


class UMasterMindInstancedSubsystem;
class EnemyObjectPool;
class ADrone;
class ASpiderbot;
class ASpawnPoints;
class UBehaviorTree;

USTRUCT(BlueprintType)
struct FEnemyWeight{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	TSubclassOf<APawn> Enemy;
	UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	UBehaviorTree* BehaviorTree;
	
	UPROPERTY(VisibleAnywhere, Category="Spawning", BlueprintReadWrite)
	int Weight;
	
	UPROPERTY(VisibleAnywhere, Category="Spawning", BlueprintReadWrite)
	float Range;
	UPROPERTY(VisibleAnywhere, Category="Spawning", BlueprintReadWrite)
	float MaxRange; 
	UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	TEnumAsByte<EEnemies> EnemyEnum; 
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_API USpawner : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawner();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY()
	UMasterMindInstancedSubsystem* MasterMind;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemyStats> ReworkList;


	/*
	UPROPERTY()
	UEnemiesEnum*  SpiderEnum;
	UPROPERTY()
	UEnemiesEnum*   DroneEnum;
	UPROPERTY()
	UEnemiesEnum*  WallBreakerEnum;
	*/
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ASpawnPoints*> SpawnLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ASpawnPoints*> PassiveSpawnPoints; 
	
	EnemyObjectPool* Pool;
	
	UFUNCTION(BlueprintCallable)
	int SpawnAtLocation(int TotalTokens);

	UFUNCTION(BlueprintCallable)
	ASpawnPoints* BestSpawnByRange(float Range,  float MaxRange ,TSubclassOf<AActor> ActorToSpawn,  UBehaviorTree* BehaviourTree, AActor* Owner);

	APawn* Spawn(TSubclassOf<AActor> ActorToSpawn,  UBehaviorTree* BehaviourTree);

	/*
	//constant values of diffrent spawn ranges
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Diffrent Ranges")
	float DroneSpawnRange = 1000;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Diffrent Ranges")
	float SpiderSpawnRange = 5000;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Diffrent Ranges")
	float WallbreakerSpawnRange = 100;
	*/

	//values of diffrent spawn  chances
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Spawn Chances")
	float SpiderSpawnChance;
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Spawn Chances")
	float DroneSpawnChance;
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Spawn Chances")
	float WallbreakerSpawnChance;

	UPROPERTY()
	int CurrentObjPoolPosition;
	
	//Time to Spawn enemy
	UPROPERTY(EditAnywhere)
	float DefaultTime;
	float Time;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float YOffset;
	
	//UFUNCTION(CallInEditor, Category="Spawning")
	void RunSpawning();

	//UFUNCTION(CallInEditor, Category="Spawning")
	void RunDelete();


	/*
	//Weight Random System
	UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	FEnemyWeight SpiderWeight;
	UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	FEnemyWeight DroneWeight;
	UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	FEnemyWeight WallBreakerWeight;
	UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	int TotalWeight = 10;

	UPROPERTY(BlueprintReadWrite)
	TArray<FEnemyWeight> WeightList;
	*/
	UFUNCTION(BlueprintCallable)
	UBehaviorTree* RandomWithWeight(FEnemyStats& Enemy, bool OverrideChance, FEnemyStats OverrideEnemy );


	//Used for random with list
	//UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	TSubclassOf<APawn> SpiderBot;
	//UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	UBehaviorTree* SpiderBT;
	
	//UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	TSubclassOf<APawn> Drone;
	//UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	UBehaviorTree* DroneBT;
	

	//UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	TSubclassOf<APawn> Wallbreaker;
	//UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	UBehaviorTree* WallbreakerBT;
	

	UFUNCTION(BlueprintCallable)
	UBehaviorTree* RandomEnemy(TSubclassOf<APawn>& Enemy, float& Range, bool OverrideChance, UEnemiesEnum* OverrideEnemy);


	UFUNCTION(CallInEditor, Category="Spawn Chances")
	void SetSpawnChances();

	UFUNCTION(BlueprintCallable)
	void ChangeSpawnChance(TArray<float> Chances, TArray<UEnemiesEnum*> Enemies);

	UFUNCTION(BlueprintCallable)
	void SpawnAtSpawnPoint(ASpawnPoints* SpawnPoint, TSubclassOf<AActor> ActorToSpawn,  UBehaviorTree* BehaviourTree, AActor* Owner);
	
private:
	//Enum for all the types of enemies


	//Data structure to hold a list of EEnemies

	UPROPERTY()
	URandomList* ListRandom;

	//spawnes an AI;
	void SpawnAI(APawn* Enemy, UBehaviorTree* BehaviourTree);
};

