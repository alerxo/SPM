// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "LocalPlayerSystem.generated.h"

/**
 * 
 */
//https://docs.unrealengine.com/4.26/en-US/ProgrammingAndScripting/Subsystems/


UCLASS()
class SPM_API ULocalPlayerSystem : public ULocalPlayer
{
	GENERATED_BODY()

public:
	



	UFUNCTION(BlueprintCallable, Category="MySubSystem")
	ULocalPlayerSystem* GetLocalPlayerSystem();
};
