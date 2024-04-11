// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SPMGameInstanceSubsystem.generated.h"

/**
 * 
 */
//https://docs.unrealengine.com/4.26/en-US/ProgrammingAndScripting/Subsystems/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpellShot, float, Amount);
UCLASS()
class SPM_API USPMGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	UPROPERTY(BlueprintAssignable)
	FOnSpellShot OnSpellShot;

	UFUNCTION(BlueprintCallable, Category="MySubSystem")
	USPMGameInstanceSubsystem* GetSPMGameInstanceSubSystem();
};
