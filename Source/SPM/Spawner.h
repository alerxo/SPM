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
	TArray<FEnemyStats> EnemyList;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ASpawnPoints*> SpawnLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ASpawnPoints*> PassiveSpawnPoints; 
	
	//EnemyObjectPool* Pool;
	
	UFUNCTION(BlueprintCallable)
	int SpawnAtLocation(int TotalTokens);

	/**
	 * Decides the first best Spawn by the MinRange And MaxRange
	 * And Then Spawns the enemy and initialize the AI. 
	 * @param Range 
	 * @param MaxRange 
	 * @param ActorToSpawn 
	 * @param BehaviourTree 
	 * @param Owner 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	ASpawnPoints* BestSpawnByRange(float Range,  float MaxRange ,TSubclassOf<AActor> ActorToSpawn,  UBehaviorTree* BehaviourTree, AActor* Owner);

	APawn* Spawn(TSubclassOf<AActor> ActorToSpawn,  UBehaviorTree* BehaviourTree);
	
	//Time to Spawn enemy
	UPROPERTY(EditAnywhere)
	float DefaultTime;
	float Time;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float YOffset;
	

	/**
	 * Give the Enemy that has been randomly chosen by weight and
	 * reference the necessary Variables
	 * @param Enemy 
	 * @param OverrideChance 
	 * @param OverrideEnemy 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	UBehaviorTree* RandomWithWeight(FEnemyStats& Enemy, bool OverrideChance, FEnemyStats OverrideEnemy );

	
	
	UFUNCTION(BlueprintCallable)
	void SpawnAtSpawnPoint(ASpawnPoints* SpawnPoint, TSubclassOf<AActor> ActorToSpawn,  UBehaviorTree* BehaviourTree, AActor* Owner);
	
private:
	//spawnes an AI;
	void SpawnAI(APawn* Enemy, UBehaviorTree* BehaviourTree);
};

