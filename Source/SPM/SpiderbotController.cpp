// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderbotController.h"


//Runs behavior tree on startup
void ASpiderbotController::BeginPlay()
{
	Super::BeginPlay();
}

void ASpiderbotController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

float ASpiderbotController::GetVisionRange() const
{
	return VisionRange;
}

bool ASpiderbotController::IsDead() const
{
	return false;
}
