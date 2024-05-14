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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyHit, FVector, Info, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSeen, FVector, info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisengage, AEnemyBaseClass*, Sender);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSentInfo, AEnemyBaseClass*, Sender, FVector, Info);

USTRUCT(Blueprintable)
struct FEnemyStats
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	int Killed;
	UPROPERTY(BlueprintReadWrite)
	int Amount;
	
	void IncrementKilled(){Killed++;};
	void IncrementAmount(){Amount++;};
};

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
	FOnEnemyHit OnEnemyHit;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDisengage OnDisengage;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPlayerSeen OnPlayerSeen;

	FOnSentInfo OnSentInfo;

	UFUNCTION(BlueprintCallable)
	bool RequestToken(APawn* Pawn);

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

	//Nummber of enemies killed
	UPROPERTY(BlueprintReadWrite)
	int SpidersKilled = 0;
	UPROPERTY(BlueprintReadWrite)
	int DronesKilled = 0;
	UPROPERTY(BlueprintReadWrite)
	int WallbreakersKilled = 0;

	//The amount for each Enemy
	UPROPERTY(BlueprintReadWrite)
	int SpiderAmount = 0;
	UPROPERTY(BlueprintReadWrite)
	int DroneAmount = 0;
	UPROPERTY(BlueprintReadWrite)
	int WallbreakerAmount = 0;

	UPROPERTY(BlueprintReadWrite)
	FEnemyStats SpiderStats;
	UPROPERTY(BlueprintReadWrite)
	FEnemyStats DroneStats;
	UPROPERTY(BlueprintReadWrite)
	FEnemyStats WallBreakerStats;

	UFUNCTION(BlueprintCallable)
	TArray<FEnemyStats> GetArrayOfStats();
	

	UFUNCTION(BlueprintCallable)
	float DivisionKilledAmount(float Killed ,float Amount){return Killed/Amount;}
};
