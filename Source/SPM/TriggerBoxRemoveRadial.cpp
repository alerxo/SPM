// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerBoxRemoveRadial.h"

void ATriggerBoxRemoveRadial::RemoveRadial()
{
	if (RadialToRemove != nullptr)
	{
		RadialToRemove->Destroy();
	}
}
