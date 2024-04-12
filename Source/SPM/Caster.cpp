// Fill out your copyright notice in the Description page of Project Settings.




#include "Caster.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DamageComponent.h"



// Sets default values for this component's properties
UCaster::UCaster()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Create DamageComponent as Subobject
	DamageComponent = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComp"));
	// ...
}


// Called when the game starts
void UCaster::BeginPlay()
{
	Super::BeginPlay();
	

	//Bind Input so you can shoot
	Input = GetOwner()->InputComponent;
	if(Input)
	{
		Input->BindAction(TEXT("DebugShoot"), IE_Pressed, this, &UCaster::Cast);
		Input->BindAction(TEXT("Debug"), IE_Pressed, this, &UCaster::SetDebug);
	}
	// ...
	
}


// Called every frame
void UCaster::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

//Casts aa spell
void UCaster::Cast() 
{
	//Create a Hit
	FHitResult Hit;
	const FVector EndLocation = GetComponentLocation() + GetForwardVector()*EndDistance;
	//Get Result of the line trace
	bool HitResult = GetWorld()->LineTraceSingleByChannel(Hit, GetComponentLocation(), EndLocation, ECC_GameTraceChannel2);
	//If there is a Hit Result Apply Damage
	if(HitResult)
	{
		if(DebugMode)
		{
			DrawDebugLine(GetWorld(), GetComponentLocation(), EndLocation,FColor::Red, false, 5);
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10, 12, FColor::Green, false, 5);
		}
		
		AActor* Owner = GetOwner();
		UGameplayStatics::ApplyDamage(Hit.GetActor(),DamageComponent->GetDamage(), Owner->GetInstigatorController(), Owner, DamageComponent->GetDamageType());

	}
}

//Set if you can see debug
void UCaster::SetDebug()
{
	DebugMode = !DebugMode;
}



