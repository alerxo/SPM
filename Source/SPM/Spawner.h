// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemieEnum.h"
#include "RandomList.h"
#include "Spawner.generated.h"


class UMasterMindInstancedSubsystem;
class EnemyObjectPool;
class ADrone;
class ASpiderbot;
class ASpawnPoints;
class UBehaviorTree;








UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_API USpawner : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawner();
	
private:


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY()
	UMasterMindInstancedSubsystem* MasterMind; 
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ASpawnPoints*> SpawnLocations;
	
	EnemyObjectPool* Pool;
	
	UFUNCTION(BlueprintCallable)
	int SpawnAtLocation(int TotalTokens);

	UFUNCTION(BlueprintCallable)
	ASpawnPoints* BestSpawnByRange(float Range, TSubclassOf<AActor> ActorToSpawn,  UBehaviorTree* BehaviourTree, AActor* Owner);

	APawn* Spawn(TSubclassOf<AActor> ActorToSpawn,  UBehaviorTree* BehaviourTree);

	//constant values of diffrent spawn ranges
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Diffrent Ranges")
	float DroneSpawnRange = 1000;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Diffrent Ranges")
	float SpiderSpawnRange = 5000;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Diffrent Ranges")
	float WallbreakerSpawnRange = 100; 

	//values of diffrent spawn  chances
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Spawn Chances")
	float SpiderSpawnChance;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Spawn Chances")
	float DroneSpawnChance;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Spawn Chances")
	float WallbreakerSpawnChance;

	UPROPERTY()
	int CurrentObjPoolPosition;
	
	//Time to Spawn enemy
	UPROPERTY(EditAnywhere)
	float DefaultTime;
	float Time;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float YOffset;
	
	UFUNCTION(CallInEditor, Category="Spawning")
	void RunSpawning();

	UFUNCTION(CallInEditor, Category="Spawning")
	void RunDelete();

	UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	TSubclassOf<APawn> SpiderBot;
	UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	UBehaviorTree* SpiderBT;
	
	UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	TSubclassOf<APawn> Drone;
	UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	UBehaviorTree* DroneBT;
	

	UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	TSubclassOf<APawn> Wallbreaker;
	UPROPERTY(EditAnywhere, Category="Spawning", BlueprintReadWrite)
	UBehaviorTree* WallbreakerBT;
	

	UFUNCTION(BlueprintCallable)
	UBehaviorTree* RandomEnemy(TSubclassOf<APawn>& Enemy, float& Range);


	UFUNCTION(CallInEditor, Category="Spawn Chances")
	void SetSpawnChances();

	UFUNCTION(BlueprintCallable)
	void ChangeSpawnChance(TArray<float> Chances, TArray<TEnumAsByte<EEnemies>> Enemies);
	
private:
	//Enum for all the types of enemies


	//Data structure to hold a list of EEnemies

	UPROPERTY()
	URandomList* ListRandom;

	//spawnes an AI;
	void SpawnAI(APawn* Enemy, UBehaviorTree* BehaviourTree);
};

