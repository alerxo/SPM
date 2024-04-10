// Fill out your copyright notice in the Description page of Project Settings.


#include "ManaComponent.h"
#include "GameplayEvent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UManaComponent::UManaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Mana = DefaultMana;

	// ...
}


// Called when the game starts
void UManaComponent::BeginPlay()
{
	Super::BeginPlay();

	Mana = DefaultMana;

	/*
	if(Event)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bind"));
		Event->OnDecreaseMana.AddDynamic(this, &UManaComponent::DecreaseMana);
	}
	*/
	
}


// Called every frame
void UManaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UManaComponent::DecreaseMana(float Amount)
{
	if(DecreaseAmount <= 0)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Decrease Mana %f"), Mana);
	if( ( Mana -= DecreaseAmount ) <= 0)
	{
		
	}
}

