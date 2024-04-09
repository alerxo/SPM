// Fill out your copyright notice in the Description page of Project Settings.


#include "RaidalActionComponent.h"

#include "ComponentUtils.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceActor.h"

// Sets default values for this component's properties
URaidalActionComponent::URaidalActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}



// Called when the game starts
void URaidalActionComponent::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision = Cast<UBoxComponent>(ComponentUtils::GetAttachedParent(this));
	ForceActor = Cast<ARadialForceActor>(GetOwner());
	if(BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &URaidalActionComponent::DisableRadial);
		BoxCollision->OnComponentEndOverlap.AddDynamic(this, &URaidalActionComponent::EnableRadial);
		UE_LOG(LogTemp, Warning, TEXT("Delegate"));
		UE_LOG(LogTemp, Warning, TEXT("Owner %s"), *GetOwner()->GetName());
	}
	// ...
}


// Called every frame
void URaidalActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	

	// ...
}
//Disable force of the raidal Actor
void URaidalActionComponent::DisableRadial(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	UE_LOG(LogTemp, Warning, TEXT("Disable %s"), *OtherActor->GetName());
	ForceActor->DisableForce();
	Count++;
}
//Enable force of the raidal Actor
void URaidalActionComponent::EnableRadial(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	UE_LOG(LogTemp, Warning, TEXT("Enable %s"), *OtherActor->GetName());
	if(Count >= 30)
	{
		return;
	}
	ForceActor->EnableForce();
	Count++;
}