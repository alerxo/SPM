// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SpiderbotShoot.generated.h"

/**
 * 
 */
UCLASS()
class SPM_API UBTTask_SpiderbotShoot : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SpiderbotShoot(); 

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
