// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomList.h"

URandomList::URandomList()
{
	TEnumAsByte<EEnemies> en = ESpider;
	RandomListChance.Init(en, 20);
	
}

URandomList::~URandomList()
{
}
