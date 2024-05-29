// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnemieEnum.h"
#include "EnemyInfo.h"
#include "MusicMaster.h"
#include "MasterMindInstancedSubsystem.generated.h"

class UBehaviorTree;
class IEnemyInterface;


//Master mind Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoundMade, FVector, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyHit, FVector, Info, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSeen, FVector, info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisengage, AEnemyBaseClass*, Sender);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSentInfo, AEnemyBaseClass*, Sender, FVector, Info);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_API UMasterMindInstancedSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	ACharacter* Player;

	UFUNCTION(BlueprintCallable)
	void SetPlayer();

public:
	UPROPERTY(BlueprintReadWrite)
	TMap<APawn*, int> MapOfTokens;

	UPROPERTY(BlueprintReadWrite)
	UMusicMaster* MusicMaster;

	UFUNCTION(BlueprintCallable)
	UMusicMaster* CreateMusicMaster();


	UPROPERTY(BlueprintReadWrite)
	int MaxTotalCoreEncounter;
	UPROPERTY(BlueprintReadWrite)
	int CurrentEnemiesSpawned;

	UPROPERTY(BlueprintReadWrite)
	TMap<int, APawn*> AllSpawnedEnemies;
	UPROPERTY(BlueprintReadWrite)
	int CurrentMapIndex; 
	
	//TQueue<APawn*> PriorityEnemies;
	//List With all the Types of Enemies and there stats
	UPROPERTY(BlueprintReadWrite)
	TArray<FEnemyStats> AllEnemyStats;

	UPROPERTY(BlueprintReadWrite)
	int MaxEnemyAmount;
	
	
	//Declare Delegate for sending Info to master mind
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSoundMade OnSoundMade;
	//Declare Delegate for when an Enemy is Hit
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEnemyHit OnEnemyHit;
	//Declare of Delegate for when the Enemies should disengage
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDisengage OnDisengage;
	//Declare Delegate for when player is spotted
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPlayerSeen OnPlayerSeen;
	//Declare Delegate for when Enemy wants to send info
	FOnSentInfo OnSentInfo;
	
	//The Decision meter to check when the Maseter mind kan do a Decision
	UPROPERTY(BlueprintReadOnly)
	int DecisionMeter;
	
	//Amount of tokens
	UPROPERTY(BlueprintReadWrite)
	int Tokens;
	
	UPROPERTY(BlueprintReadWrite)
	int  DefaultTokens;

	UPROPERTY(BlueprintReadWrite)
	int TotalEnemyAmount;
	UPROPERTY(BlueprintReadOnly)
	double TotalEnemyWeight;
	
	FVector InvestigationLocation;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	 * Adds to the Token Limit
	 * @param MultiplyProcentage 
	 */
	UFUNCTION(BlueprintCallable)
	void AddTokensLimit(float MultiplyProcentage);
	/**
	 * Reduces the Token Limit
	 * @param MultiplyProcentage 
	 */
	UFUNCTION(BlueprintCallable)
	void ReduceTokenLimit(float MultiplyProcentage);
	

	/**
	 * Checks if the Pawn has ha token if it does it removes it and reduces the amount of tokens
	 * @param Amount 
	 * @param Pawn 
	 */
	UFUNCTION(BlueprintCallable)
	void CheckAndDeleteToken(TEnumAsByte<EEnemies> EnemyType, APawn* Pawn);
	
	/**
	 * Increases the decision meter a certain amount
	 * @param Amount 
	 */
	UFUNCTION(BlueprintCallable)
	void IncreaseDecisionMeter(int Amount);

	/**
	 * The Enemy hands back the number of Tokens
	 * @param Amount 
	 */
	UFUNCTION(BlueprintCallable)
	void HandBackToken(int Amount, APawn* Pawn );
	/**
	 * Resets the Decision Meter to 0
	 */
	UFUNCTION(BlueprintCallable)
	void ResetDecisionMeter();
	/**
	 * Request A token to check if enemy can shoot
	 * @param Pawn 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	bool RequestToken(APawn* Pawn);
	/**
	 * Decreases the Token Amount
	 * @param Amount 
	 */
	UFUNCTION(BlueprintCallable)
	void DecreaseTokens(int Amount);
	/**
	 * Increases The Token Amount
	 * @param Amount 
	 */
	UFUNCTION(BlueprintCallable)
	void IncreaseTokens(int Amount);
	/**
	 * Goes trough all the enemy types and updates the Weight
	 */
	UFUNCTION(BlueprintCallable)
	void UpdateWeightAllAtOnce();
	/**
	 * Updates The weight with an Amount
	 * @param Amount 
	 */
	UFUNCTION(BlueprintCallable)
	void UpdateWeight(double Amount);
	
	UFUNCTION(BlueprintCallable)
	FVector GetInvestigationLocation() const;
	
	UFUNCTION(BlueprintCallable)
	void SetInvestigationLocation(FVector Vector);

	/**
	 * Getts the Master Mind Subsystem
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category="MySubSystem")
	UMasterMindInstancedSubsystem* GetMasterMindInstancedSubsystem();
	
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
	/**
	 * Increase the Amount count of the EnemyType
	 * @param Enemy 
	 */
	UFUNCTION(BlueprintCallable)
	void IncreasEnemyAmount(TEnumAsByte<EEnemies> Enemy);
	/**
	 * Decreases the enemy amount of the Enemy Type
	 * @param Enemy 
	 */
	UFUNCTION(BlueprintCallable)
	void DecreaseEnemyAmount(TEnumAsByte<EEnemies> Enemy);
	/**
	 * Resets the Weight to 0
	 */
	UFUNCTION(BlueprintCallable)
	void ResetWeight();
	/**
	 * Increases the kill count of the Enemy type
	 * @param Enemy 
	 */
	UFUNCTION(BlueprintCallable)
	void IncreaseEnemyKilled(TEnumAsByte<EEnemies> Enemy);
	/**
	 * Creates Enemy Stats in AllEnemyStats List
	 * @param Weight 
	 * @param Enemy 
	 */
	UFUNCTION(BlueprintCallable)
	void CreateEnemyStats(FEnemyStats EnemyStats);
	/**
	 * Takes the Enemy Type and changes The Weight Based on Killed And Damage
	 * @param Enemy 
	 */
	UFUNCTION(BlueprintCallable)
	double BalanceKilledAndDamage(TEnumAsByte<EEnemies> Enemy);
	/**
	 * Calculates the procentage of weight that an enemy type holds 
	 * @param Enemy 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	double WeightProcentageOfEnemy(	TEnumAsByte<EEnemies> Enemy);
};
