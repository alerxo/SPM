// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnemieEnum.h"
#include "MasterMindInstancedSubsystem.generated.h"

class IEnemyInterface;
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
	UPROPERTY(BlueprintReadWrite)
	int TotalHits;
	UPROPERTY(BlueprintReadWrite)
	double Weight;
	UPROPERTY(BlueprintReadWrite)
	float DamageAmount;
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

	UPROPERTY(BlueprintReadOnly)
	double TotalEnemyWeight;
	UPROPERTY(BlueprintReadOnly)
	int TotalEnemyAmount;

	UFUNCTION(BlueprintCallable)
	void UpdateWeightAllAtOnce();
	UFUNCTION(BlueprintCallable)
	void UpdateWeight(double Amount);
	
	UFUNCTION(BlueprintCallable)
	FVector GetInvestigationLocation() const;
	
	UFUNCTION(BlueprintCallable)
	void SetInvestigationLocation(FVector Vector);

	FVector InvestigationLocation;
	
	UFUNCTION(BlueprintCallable, Category="MySubSystem")
	UMasterMindInstancedSubsystem* GetMasterMindInstancedSubsystem();

	/*
	//Nummber of enemies killed
	UPROPERTY(BlueprintReadWrite)
	int SpidersKilled = 0;
	UPROPERTY(BlueprintReadWrite)
	int DronesKilled = 0;
	UPROPERTY(BlueprintReadWrite)
	int WallbreakersKilled = 0;
*/
	//The amount for each Enemy
	/*
	UPROPERTY(BlueprintReadWrite)
	int SpiderAmount = 0;
	UPROPERTY(BlueprintReadWrite)
	int DroneAmount = 0;
	UPROPERTY(BlueprintReadWrite)
	int WallbreakerAmount = 0;
*/
	/*
	UPROPERTY(BlueprintReadWrite)
	FEnemyStats SpiderStats;
	UPROPERTY(BlueprintReadWrite)
	FEnemyStats DroneStats;
	UPROPERTY(BlueprintReadWrite)
	FEnemyStats WallBreakerStats;

	UFUNCTION(BlueprintCallable)
	TArray<FEnemyStats> GetArrayOfStats();
	*/

	UFUNCTION(BlueprintCallable)
	void SetUp();

	UPROPERTY()
	UEnemiesEnum*  SpiderEnum;
	UPROPERTY()
	UEnemiesEnum*   DroneEnum;
	UPROPERTY()
	UEnemiesEnum*  WallBreakerEnum;

	/*
	UFUNCTION(BlueprintCallable)
	TArray<UEnemiesEnum*> GetListOfAllEnemiesTypes(){return ListOfAllEnemieEnum;}
	*/

	UFUNCTION(BlueprintCallable)
	void Hit(UEnemiesEnum* Enemy);

	UFUNCTION(BlueprintCallable)
	float DivisionKilledAmount(float Killed ,float Amount){return Killed/Amount;}

	/**
	 * Send The interface Of the Class of Type IEnemyInterface,
	 * Will Return the EEnemeis type it is
	 *
	 * The Enemy Needs To Set The EnemyType
	 * @param GeneralEnemy 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EEnemies> GetEnemyType(TScriptInterface<IEnemyInterface> GeneralEnemy);

	UPROPERTY(BlueprintReadWrite)
	TArray<FEnemyStats> AllEnemyStats;

	/**
	 * Increases The General Hits done to Player by the type of Enemy
	 * @param Enemy 
	 */
	UFUNCTION(BlueprintCallable)
	void IncreaseHit(TEnumAsByte<EEnemies> Enemy);
	/**
	 * Increases The General Weight done to Player by the type of Enemy
	 * @param Enemy
	 * @param Amount
	 */
	UFUNCTION(BlueprintCallable)
	void IncreaseWeight(TEnumAsByte<EEnemies> Enemy, double Amount);
	/**
	 * Increase The Total Damage Amount for the Enemy Type
	 * @param Enemy 
	 * @param Amount 
	 */
	UFUNCTION(BlueprintCallable)
	void IncreaseDamageAmount(TEnumAsByte<EEnemies> Enemy, float Amount);

	UFUNCTION(BlueprintCallable)
	void IncreasEnemyAmount(TEnumAsByte<EEnemies> Enemy);
	UFUNCTION(BlueprintCallable)
	void IncreasaEnemyAmount(FEnemyStats& EnemyStats);
	UFUNCTION(BlueprintCallable)
	void IncreaseEnemyKilled(TEnumAsByte<EEnemies> Enemy);
	UFUNCTION(BlueprintCallable)
	FEnemyStats CreateEnemyStats(double Weight);

	/**
	 * Takes the Enemy Type and changes The Weight Bassed on Killed And Damage
	 * @param Enemy 
	 */
	UFUNCTION(BlueprintCallable)
	double BalanceKilledAndDamage(TEnumAsByte<EEnemies> Enemy);
};
