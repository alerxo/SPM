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

	UPROPERTY(EditAnywhere)
	TArray<ABaseCore*> Cores;
	
	UPROPERTY(EditAnywhere)
	AInteractableCoreTerminal* CoreTerminal;

	UPROPERTY()
	class USPMGameInstanceSubsystem* GameInstanceSubsystem;
	
	UFUNCTION()
	void CheckCores(ABaseCore* Core);

};
