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
	virtual void BeginPlay() override;

	void Interact() override;
public:
	UPROPERTY(EditAnywhere)
	ABaseCore* Core; 
	
};
