// Fill out your copyright notice in the Description page of Project Settings.


#include "Caster.h"

// Sets default values for this component's properties
UCaster::UCaster()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCaster::BeginPlay()
{
	Super::BeginPlay();

	
	Input = GetOwner()->InputComponent;
	if(Input)
	{
		Input->BindAction(TEXT("Shoot"), IE_Pressed, this, &UCaster::Cast);
	}

	
	// ...
	
}


// Called every frame
void UCaster::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCaster::Cast() 
{
	FHitResult Hit;
	FVector EndLocation = GetComponentLocation();
	EndLocation.X += EndDistance;
	if(GetWorld()->LineTraceSingleByChannel(Hit, GetComponentLocation(), EndLocation, ECC_EngineTraceChannel2))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit"));
	}
}


