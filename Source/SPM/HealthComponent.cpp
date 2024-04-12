// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	//Dynamically Add TakeDamage Method to OnTakeDamage Delegate
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	
	DefaultHealth = Health;
	
}


// Called when the event for Taking Damage OnTakeAnyDamage Delegate
void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Health %f"), DefaultHealth);
	if(Damage <= 0)
	{
		return;
	}
	//Damage the Owner(Actor)
	if((DefaultHealth -= Damage) <= 0)
	{
		DefaultHealth = 0;
	}
}

//Getter for getting Health
float UHealthComponent::GetHealth() const
{
		return DefaultHealth;
}
//Getter for The Health Precentage
float UHealthComponent::GetHealthPercentage() const
{
	return DefaultHealth / Health;
}
