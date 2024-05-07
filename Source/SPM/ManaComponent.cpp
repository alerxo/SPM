// Fill out your copyright notice in the Description page of Project Settings.


#include "ManaComponent.h"
#include "SPMGameInstanceSubsystem.h"

// Sets default values for this component's properties
UManaComponent::UManaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Mana = DefaultMana;
	ElectricMana = DefaultMana;


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

	//Create GameInstanceSubSystem to call delegates
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	Subsystem = GameInstance->GetSubsystem<USPMGameInstanceSubsystem>();
	
	if(Subsystem)
	{
		//Dynamically Add Decrease mana to OnSpellShot Delegate
		Subsystem->OnSpellShot.AddDynamic(this, &UManaComponent::DecreaseMana);
	}
	//Set float in Timer 
	Timer = DefaultTimer;
	ElectricTimer = DefaultTimer;
	//Set float in Mana
	Mana = DefaultMana;
	ElectricMana = DefaultMana;


	
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

	if(bElectricCanRecharge)
	{
		RechargeElectricMana(DeltaTime);
	}

	// ...
}

//Recharges mana 
void UManaComponent::RechargeMana(float DeltaTime)
{
	if(Timer <= 0)
	{
		Mana += 1;
	
		if(Mana >= DefaultMana)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Full Recharged %f"), Mana);
			Mana = DefaultMana;
			bCanRecharge = false;
		}
		
	} 
	Timer -= DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("%f"), Timer);
}

void UManaComponent::RechargeElectricMana(float DeltaTime)
{
	if(ElectricTimer <= 0)
	{
		ElectricMana += 1;
		if(ElectricMana >= DefaultMana)
		{
			ElectricMana;
			bElectricCanRecharge = false;
		}
	}
	ElectricTimer -= DeltaTime;
}


//Decrease mana When called
void UManaComponent::DecreaseMana(float Amount)
{
	
	if(Amount <= 0)
	{
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Decrease Mana %f"), Mana);
	bCanRecharge = true;
	Timer = DefaultTimer;
	if( ( Mana -= Amount ) <= 0)
	{
		Mana = 0;
	}
	
}

void UManaComponent::DecreaseElectricMana(float Amount)
{
	if(Amount <= 0)
	{
		return;
	}
	bElectricCanRecharge = true;
	ElectricTimer = DefaultTimer;
	if((ElectricMana -= Amount) <= 0)
	{
		ElectricMana = 0;
	}
}


//Getter float of mana percentage
float UManaComponent::GetManaPercent() const
{
	return Mana / DefaultMana;
}

float UManaComponent::GetElectricManaPercent() const
{
	return ElectricMana / DefaultMana;
}



