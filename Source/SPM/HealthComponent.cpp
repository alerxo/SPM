// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "Kismet/GameplayStatics.h"

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
	ActivateTakeDamage();
	
	DefaultHealth = Health;
	
}

void UHealthComponent::DisableTakeDamage()
{
	GetOwner()->OnTakeAnyDamage.RemoveDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::ActivateTakeDamage()
{
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::SetHealthNew(float NewHealth)
{
	if (NewHealth >= Health)
	{
		Health = DefaultHealth;
	}
	else
	{
		DefaultHealth = NewHealth;
	}
}

// Called when the event for Taking Damage OnTakeAnyDamage Delegate
void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	
	if (DamagedActor == Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetOwner(), 0)) || DamageCauser == Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetOwner(), 0)))
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
			OnDead.Broadcast();
		}
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

void UHealthComponent::RestoreHealth(float HealthToRestore)
{
	DefaultHealth += HealthToRestore;
	if (DefaultHealth > Health)
	{
		DefaultHealth = Health;
	}
}

float  UHealthComponent::GetDefaultHealth()const
{
	return Health;
};