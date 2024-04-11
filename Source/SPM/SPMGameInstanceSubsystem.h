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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoreDestroyed, bool, IsInteractable);
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
	FOnCoreDestroyed OnCoreDestroyed;

	UPROPERTY(BlueprintAssignable)
	FOnSpellShot OnSpellShot;

	UPROPERTY(BlueprintAssignable)
	FOnInteractable OnInteractable;

	UFUNCTION(BlueprintCallable, Category="MySubSystem")
	USPMGameInstanceSubsystem* GetSPMGameInstanceSubSystem();
};
