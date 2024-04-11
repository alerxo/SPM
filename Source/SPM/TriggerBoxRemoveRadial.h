// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "TriggerBoxRemoveRadial.generated.h"

/**
 * 
 */
UCLASS()
class SPM_API ATriggerBoxRemoveRadial : public ATriggerBox
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	AActor* RadialToRemove;

	UFUNCTION(BlueprintCallable)
	void RemoveRadial();
};
