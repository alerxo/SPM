// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemieEnum.h"
#include "GameFramework/DamageType.h"
#include "EnemyDamageType.generated.h"

/**
 * 
 */
UCLASS()
class SPM_API UEnemyDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Type")
	TEnumAsByte<EEnemies> EnemyType;
	
};
