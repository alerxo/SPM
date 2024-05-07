// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MasterMindInstancedSubsystem.generated.h"

/**
 * 
 */
//Master mind Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoundMade, FVector, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSeen, FVector, info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisengage, AEnemyBaseClass*, Sender);
UCLASS()
class SPM_API UMasterMindInstancedSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	//Declare Delegate for sending Info to master mind 
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSoundMade OnSoundMade;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDisengage OnDisengage;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPlayerSeen OnPlayerSeen;

	UFUNCTION(BlueprintCallable)
	bool RequestToken();

	//getters and setters
	UFUNCTION(BlueprintCallable)
	void SetTokens(float Amount);

	UFUNCTION(BlueprintCallable)
	float GetTokens();
	
	//Amount of tokens
	float Tokens;
	
	UFUNCTION(BlueprintCallable)
	FVector GetInvestigationLocation() const;
	
	UFUNCTION(BlueprintCallable)
	void SetInvestigationLocation(FVector Vector);

	FVector InvestigationLocation;
	
	UFUNCTION(BlueprintCallable, Category="MySubSystem")
	UMasterMindInstancedSubsystem* GetMasterMindInstancedSubsystem();
};
