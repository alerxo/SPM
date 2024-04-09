


#include "MyRadialForceComponent.h"

#include "DestructibleInterface.h"
#include "GameFramework/MovementComponent.h"

UMyRadialForceComponent::UMyRadialForceComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UpdateCollisionObjectQueryParams();
}


void UMyRadialForceComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(IsActive())
	{
		const FVector Origin = GetComponentLocation() - FVector(0,0,Radius);

		// Find objects within the Capsule
		TArray<FOverlapResult> Overlaps;

		FCollisionQueryParams Params(SCENE_QUERY_STAT(AddForceOverlap), false);

		// Ignore owner actor if desired
		if (bIgnoreOwningActor)
		{
			Params.AddIgnoredActor(GetOwner());
		}

		GetWorld()->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, CollisionObjectQueryParams,FCollisionShape::MakeCapsule(Radius,Height/2), Params);
		
		DrawDebugCapsule(GetWorld(),Origin, Height, Radius, FQuat::Identity, FColor::Red, false);
		// A component can have multiple physics presences (e.g. destructible mesh components).
		// The component should handle the radial force for all of the physics objects it contains
		// so here we grab all of the unique components to avoid applying impulses more than once.
		TArray<UPrimitiveComponent*, TInlineAllocator<1>> AffectedComponents;
		AffectedComponents.Reserve(Overlaps.Num());

		for(FOverlapResult& OverlapResult : Overlaps)
		{
			
			if(UPrimitiveComponent* PrimitiveComponent = OverlapResult.Component.Get())
			{
				AffectedComponents.AddUnique(PrimitiveComponent);
			}
		}

		for(UPrimitiveComponent* PrimitiveComponent : AffectedComponents)
		{
			PrimitiveComponent->AddRadialForce(Origin, Radius, ForceStrength, Falloff);

			// see if this is a target for a movement component
			AActor* ComponentOwner = PrimitiveComponent->GetOwner();
			if(ComponentOwner)
			{
				TInlineComponentArray<UMovementComponent*> MovementComponents;
				ComponentOwner->GetComponents(MovementComponents);
				for(const auto& MovementComponent : MovementComponents)
				{
					if(MovementComponent->UpdatedComponent == PrimitiveComponent)
					{
						UE_LOG(LogTemp, Warning, TEXT("WORKING"));
						
						MovementComponent->AddRadialForce(Origin, Radius, ForceStrength, Falloff);
						break;
					}
				}
			}
		}
	}
}


void UMyRadialForceComponent::FireImpulse()
{
	const FVector Origin = GetComponentLocation();

	// Find objects within the Capsule
	TArray<FOverlapResult> Overlaps;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(FireImpulseOverlap),  false);

	// Ignore owner actor if desired
	if (bIgnoreOwningActor)
	{
		Params.AddIgnoredActor(GetOwner());
	}

	GetWorld()->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeCapsule(Radius,Height), Params);



	// A component can have multiple physics presences (e.g. destructible mesh components).
	// The component should handle the radial force for all of the physics objects it contains
	// so here we grab all of the unique components to avoid applying impulses more than once.
	TArray<UPrimitiveComponent*, TInlineAllocator<1>> AffectedComponents;
	AffectedComponents.Reserve(Overlaps.Num());

	for(FOverlapResult& OverlapResult : Overlaps)
	{
		if(UPrimitiveComponent* PrimitiveComponent = OverlapResult.Component.Get())
		{
			AffectedComponents.AddUnique(PrimitiveComponent);
		}
	}

	for(UPrimitiveComponent* PrimitiveComponent : AffectedComponents)
	{
		if(DestructibleDamage > UE_SMALL_NUMBER)
		{
			if(IDestructibleInterface* DestructibleInstance = Cast<IDestructibleInterface>(PrimitiveComponent))
			{
				DestructibleInstance->ApplyRadiusDamage(DestructibleDamage, Origin, Radius, ImpulseStrength, Falloff == RIF_Constant);
			}
		}

		// Apply impulse
		PrimitiveComponent->AddRadialImpulse(Origin, Radius, ImpulseStrength, Falloff, bImpulseVelChange);

		// See if this is a target for a movement component, if so apply the impulse to it
		if (PrimitiveComponent->bIgnoreRadialImpulse == false)
		{
			TInlineComponentArray<UMovementComponent*> MovementComponents;
			if(AActor* OwningActor = PrimitiveComponent->GetOwner())
			{
				OwningActor->GetComponents(MovementComponents);
				for(const auto& MovementComponent : MovementComponents)
				{
					if(MovementComponent->UpdatedComponent == PrimitiveComponent)
					{
						MovementComponent->AddRadialImpulse(Origin, Radius, ImpulseStrength, Falloff, bImpulseVelChange);
						break;
					}
				}
			}
		}
	}
}

void UMyRadialForceComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdateCollisionObjectQueryParams();
}

void UMyRadialForceComponent::PostLoad()
{
	Super::PostLoad();

	UpdateCollisionObjectQueryParams();
}
