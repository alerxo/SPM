// Fill out your copyright notice in the Description page of Project Settings.


#include "Caster.h"

#include "CollisionDebugDrawingPublic.h"
#include "KismetTraceUtils.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCaster::UCaster()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DamageComponent = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComp"));
	// ...
}


// Called when the game starts
void UCaster::BeginPlay()
{
	Super::BeginPlay();

	
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
	
	FHitResult Hit;
	const FVector EndLocation = GetComponentLocation() + GetForwardVector()*EndDistance;
	bool HitResult = GetWorld()->LineTraceSingleByChannel(Hit, GetComponentLocation(), EndLocation, ECC_GameTraceChannel2); 
	if(HitResult)
	{
		if(DebugMode)
		{
			DrawDebugLine(GetWorld(), GetComponentLocation(), EndLocation,FColor::Red, false, 5);
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10, 12, FColor::Green, false, 5);
		}

		if(Event)
		{
			Event->OnDecreaseMana.Broadcast(10);
		}
		AActor* Owner = GetOwner();
		UGameplayStatics::ApplyDamage(Hit.GetActor(),DamageComponent->GetDamage(), Owner->GetInstigatorController(), Owner, DamageComponent->GetDamageType());

	}
}

void UCaster::SetDebug()
{
	DebugMode = !DebugMode;
}



