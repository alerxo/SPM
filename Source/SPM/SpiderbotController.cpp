// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderbotController.h"

void ASpiderbotController::BeginPlay()
{
	Super::BeginPlay();

	if (SpiderbotBehavior != nullptr)
	{
		RunBehaviorTree(SpiderbotBehavior);
	}
}

void ASpiderbotController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

