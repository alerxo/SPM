// Fill out your copyright notice in the Description page of Project Settings.


#include "ManaComponent.h"
#include "PlayerStateListener.h"
#include "SPMGameInstanceSubsystem.h"
#include "GameFramework/Character.h"
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

	/*
	if(ACharacter* Owner = Cast<ACharacter>(GetOwner()))
	{
		GameplayEvent =  Cast<APlayerStateListener>(Owner->GetPlayerState());
		UE_LOG(LogTemp,Warning, TEXT("BINDED"))
	}
	*/


	UGameInstance* GameInstance = GetWorld()->GetGameInstance();

	Subsystem = GameInstance->GetSubsystem<USPMGameInstanceSubsystem>();
	if(Subsystem)
	{
		Subsystem->OnLocalTest.AddDynamic(this, &UManaComponent::DecreaseMana);
	}
	//Set float in Timer 
	Timer = DefaultTimer;
	//Set float in Mana
	Mana = DefaultMana;


	
	//GameplayEvent->OnDecreaseMana.AddDynamic(this, &UManaComponent::DecreaseMana);
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

	
	if(bCanRecharge)
	{
		RechargeMana(DeltaTime);
	}

	// ...
}

void UManaComponent::RechargeMana(float DeltaTime)
{
	if(Timer <= 0)
	{
		Mana += 1;
	
		if(Mana >= DefaultMana)
		{
			UE_LOG(LogTemp, Warning, TEXT("Full Recharged %f"), Mana);
			Mana = DefaultMana;
			bCanRecharge = false;
		}
		
	} 
	Timer -= DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("%f"), Timer);
}



void UManaComponent::DecreaseMana(float Amount)
{
	
	if(DecreaseAmount <= 0)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Decrease Mana %f"), Mana);
	bCanRecharge = true;
	Timer = DefaultTimer;
	if( ( Mana -= DecreaseAmount ) <= 0)
	{
		
	}
	
}

