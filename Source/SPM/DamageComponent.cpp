// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageComponent.h"


// Sets default values for this component's properties
UDamageComponent::UDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<UDamageType> test = GetDamageType();
	// ...
	
}


float UDamageComponent::GetDamage() const
{
	return Damage;
}

TSubclassOf<UDamageType> UDamageComponent::GetDamageType() const
{
	if(DamageType)
	{
		return  DamageType;
	}
	UE_LOG(LogTemp, Error, TEXT("No DamageType Selected : %s"), *GetOwner()->GetName());
	return DamageType;
}
/*
*/


