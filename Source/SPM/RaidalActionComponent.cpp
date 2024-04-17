// Fill out your copyright notice in the Description page of Project Settings.


#include "RaidalActionComponent.h"

#include "ComponentUtils.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceActor.h"
#include "PhysicsEngine/RadialForceComponent.h"

//////////////////////////////////////////
//CLASS IS JUST TEST AND WILL BE DELETED//
/////////////////////////////////////////
//MyRadialForceComponent is used instead

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

	BoxCollision = Cast<UBoxComponent>(ComponentUtils::GetAttachedParent(this)->GetAttachParent());
	EndBoxCollision = Cast<UBoxComponent>(ComponentUtils::GetAttachedParent(this));
	
	ForceActor = Cast<ARadialForceActor>(GetOwner());
	if(BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &URaidalActionComponent::DisableRadial);
		
		UE_LOG(LogTemp, Warning, TEXT("Delegate"));
		UE_LOG(LogTemp, Warning, TEXT("Owner %s"), *GetOwner()->GetName());
	}
	// ...
}


// Called every frame
void URaidalActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bPlayerIn)
	{
		
	}
	

	// ...
}
//Disable force of the raidal Actor
void URaidalActionComponent::DisableRadial(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	UShapeComponent* Shape = Cast<UShapeComponent>(OtherComp);
	if(Shape)
	{
	
		//ForceActor->DisableForce();
		//Shape->SetSimulatePhysics(true);
		ForceActor->GetForceComponent()->ForceStrength = -150000;
		//Shape->SetLinearDamping(FallDamping);
		BoxCollision->OnComponentBeginOverlap.RemoveDynamic(this, &URaidalActionComponent::DisableRadial);
		EndBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &URaidalActionComponent::EnableRadial);
		//Shape->OnComponentHit.AddDynamic(this, &URaidalActionComponent::EnableRadial);
	}
}
//Enable force of the raidal Actor
void URaidalActionComponent::EnableRadial(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	//Stäng av damping
	UShapeComponent* Shape = Cast<UShapeComponent>(OtherComp);
	ForceActor->GetForceComponent()->Deactivate();
	if(Shape)
	{

		//	GetCharacterMovement()->ClearAccumulatedForces();
		//ACharacter* s = Cast<ACharacter>(Shape->GetOwner());
		//Shape->SetLinearDamping(0.01);
		//Shape->SetSimulatePhysics(false);
		//UE_LOG(LogTemp, Warning, TEXT("Enable"))
		//EndBoxCollision->OnComponentBeginOverlap.RemoveDynamic(this, &URaidalActionComponent::EnableRadial);
		
	}
	
}

