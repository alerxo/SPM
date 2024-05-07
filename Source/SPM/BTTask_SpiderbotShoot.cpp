// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SpiderbotShoot.h"

#include "AIController.h"
#include "Spiderbot.h"

UBTTask_SpiderbotShoot::UBTTask_SpiderbotShoot()
{
	NodeName = TEXT("Shoot");
}

//Executes shoot method in Spiderbot class
EBTNodeResult::Type UBTTask_SpiderbotShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ASpiderbot* Spiderbot = Cast<ASpiderbot>(OwnerComp.GetAIOwner()->GetPawn());

	if (Spiderbot == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	//Spiderbot->Fire();

	return EBTNodeResult::Succeeded;
}