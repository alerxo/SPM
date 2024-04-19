// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "InteractableCoreTerminal.generated.h"

class ABaseCore;
class USPMGameInstanceSubsystem;
/**
 * 
 */

UCLASS()
class SPM_API AInteractableCoreTerminal : public AInteractable
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Called When Player Interacts
	void Interact() override;

	
public:
	//Terminal is active
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive;

	//name of level to call
	UPROPERTY(EditAnywhere)
	FName LevelName; 

	//Setter Terminal is Active
	UFUNCTION()
	void SetIsActive(bool Value);

	//Getter bool IsActive
	bool GetIsActive()const{return bIsActive;}

	//Array of Cores
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<ABaseCore*> Cores;

	//GameInstanceSubSystem to Call Delegates
	UPROPERTY()
	class USPMGameInstanceSubsystem* GameInstanceSubsystem;

	//function to remove core in array and check it
	UFUNCTION()
	void CheckCores(ABaseCore* Core);
	
};
