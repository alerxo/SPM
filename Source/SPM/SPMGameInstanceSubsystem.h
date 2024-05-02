// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SPMGameInstanceSubsystem.generated.h"

/**
 * 
 */
//https://docs.unrealengine.com/4.26/en-US/ProgrammingAndScripting/Subsystems/
class ABaseCore;
//Declare Diffrent Delegates 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpellShot, float, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoreDeactivated, ABaseCore*, Core);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoreDestroyed, ABaseCore*, Core);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoreActivated, ABaseCore*, Core);



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

	//Declare Delegate Class FOnSpellShot
	UPROPERTY(BlueprintAssignable)
	FOnSpellShot OnSpellShot;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnCoreDeactivated OnCoreDeactivated;

	//Call when Core is Activated
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnCoreActivated OnCoreActivated;
	
	//Declare Delegate FOnInteractable
	UPROPERTY(BlueprintAssignable)
	FOnInteractable OnInteractable;

	//Getter For GameInstanceSubSystem
	UFUNCTION(BlueprintCallable, Category="MySubSystem")
	USPMGameInstanceSubsystem* GetSPMGameInstanceSubSystem();
};
