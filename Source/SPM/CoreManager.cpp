// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreManager.h"

#include "InteractableCoreTerminal.h"
#include "SPMGameInstanceSubsystem.h"

// Sets default values
ACoreManager::ACoreManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CoreTerminal = nullptr;
}

// Called when the game starts or when spawned
void ACoreManager::BeginPlay()
{
	Super::BeginPlay();
	//Create GameInstanceSubSystem to call delegates
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	GameInstanceSubsystem = GameInstance->GetSubsystem<USPMGameInstanceSubsystem>();

	if(GameInstanceSubsystem)
	{
		//Dynamically add CheckCores to OnCoreDestroyed
		GameInstanceSubsystem->OnCoreDestroyed.AddDynamic(this, &ACoreManager::CheckCores);
	}


	
}

// Called every frame
void ACoreManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Method to Check the Cores Array and remove chosen core
void ACoreManager::CheckCores(ABaseCore* Core)
{
	if(CoreTerminal)
	{
		UE_LOG(LogTemp, Error, TEXT("No CoreTerminal in Scene"));
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("Check Cores"));
	Cores.Remove(Core);
	//if null then Terminal is active
	if(Cores.Num() == 0)
	{
		CoreTerminal->SetIsActive(true);
	}
}


