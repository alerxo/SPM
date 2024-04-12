// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreManager.generated.h"

class AInteractableCoreTerminal;
class ABaseCore;
UCLASS()
class SPM_API ACoreManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoreManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Array of cores
	UPROPERTY(EditAnywhere)
	TArray<ABaseCore*> Cores;

	//The terminal that hold the cores
	UPROPERTY(EditAnywhere)
	AInteractableCoreTerminal* CoreTerminal;

	//GameInstanceSubSystem
	UPROPERTY()
	class USPMGameInstanceSubsystem* GameInstanceSubsystem;
	
	//Function to check a core and remove from array
	UFUNCTION()
	void CheckCores(ABaseCore* Core);

};
