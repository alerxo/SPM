// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemieEnum.h"

UEnemiesEnum::UEnemiesEnum()
{
	
}

UEnemiesEnum::UEnemiesEnum(TEnumAsByte<EEnemies> EnemyType)
{
	ListOfAllEnemieEnum[EnemyType.GetValue()] = EnemyType;
}

UEnemiesEnum::~UEnemiesEnum()
{
	
}
/*
UEnemiesEnum::UEnemiesEnum(const UEnemiesEnum& other)
{
	Enemy = other.Enemy;
}
*/

TEnumAsByte<EEnemies> UEnemiesEnum::GetValue()
{
	return this->Enemy;
}

