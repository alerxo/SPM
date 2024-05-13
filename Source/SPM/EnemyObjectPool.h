// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

class SPM_API EnemyObjectPool 
{
public:
	EnemyObjectPool();
	~EnemyObjectPool();

	TArray<APawn*> SpiderBot;
	TArray<TSubclassOf<APawn>> Drone;
	TArray<TSubclassOf<APawn>> Wallbreaker;
};
