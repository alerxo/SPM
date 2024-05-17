// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MasterMind.generated.h"

class AEnemyBaseClass;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnOnCore, int, TotalTokens);
UCLASS()
class SPM_API AMasterMind : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterMind();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSpawnOnCore OnSpawnOnCore;

	//Enemy sends info to Master mind
	UFUNCTION(BlueprintCallable)
	void SendInfo(AEnemyBaseClass* Sender, FVector info);

	//Master mind Reports to nearby enemies
	UFUNCTION(BlueprintCallable)
	void ReportToOthers(FVector info);

	UFUNCTION()
	void print(int token);
};
