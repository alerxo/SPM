// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "InteractableCoreTerminal.generated.h"

class USPMGameInstanceSubsystem;
/**
 * 
 */
UCLASS()
class SPM_API AInteractableCoreTerminal : public AInteractable
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	void Interact() override;

	
public:
	bool bIsActive;

	UPROPERTY(EditAnywhere)
	FName LevelName; 
	
	UFUNCTION()
	void SetIsActive(bool Value);
	
	UPROPERTY()
	USPMGameInstanceSubsystem* GameInstanceSubsystem;
};
