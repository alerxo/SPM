// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Spawner.generated.h"


class ASpawnPoints;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ASpawnPoints*> SpawnLocations;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> EnemyObjectPool;
	
	UFUNCTION(BlueprintCallable)
	int SpawnAtLocation(int TotalTokens);


	UPROPERTY()
	int CurrentObjPoolPosition;

	
	//Time to Spawn enemy
	UPROPERTY(EditAnywhere)
	float DefaultTime;

	float Time;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float YOffset;
};
