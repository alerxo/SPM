// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "InteractableTerminal.generated.h"

class ABaseCore;
/**
 * 
 */

UCLASS()
class SPM_API AInteractableTerminal : public AInteractable
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Called When Player Interacts
	void Interact() override;
public:
	
	//The Core the terminal is connected to
	UPROPERTY(EditAnywhere)
	ABaseCore* Core;
	
};
