// Fill out your copyright notice in the Description page of Project Settings.


#include "FireballProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "MasterMindInstancedSubsystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFireballProjectile::AFireballProjectile()
{

	DamageComponent = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComponent"));
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddUniqueDynamic(this, &AFireballProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AFireballProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentHit.AddUniqueDynamic(this, &AFireballProjectile::OnHit);

}

void AFireballProjectile::Explode()
{
	TArray<FHitResult> OutHits;

	FVector HitLocation = GetActorLocation();
		
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(ExplosiveRadius);
	//DrawDebugSphere(GetWorld(), HitLocation, CollisionSphere.GetSphereRadius(), 25, FColor::Red, true);
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, HitLocation, HitLocation,
		FQuat::Identity, ECC_WorldStatic, CollisionSphere);

	if(isHit)
	{
		for(FHitResult& Hit : OutHits)
		{
			if(Hit.GetActor() != nullptr)
			{
				DisablePlayerCollision(Hit);
				UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());
				UGameplayStatics::ApplyDamage(Hit.GetActor(), DamageComponent->GetDamage(), this->GetInstigatorController(), this, DamageComponent->GetDamageType());
				if(MeshComponent)
				{
					MeshComponent->AddRadialImpulse(HitLocation, ExplosiveRadius, ExplosiveImpulseStrength, RIF_Constant, true);
				
				}
				DestroyWithFireball();	
			}
		}
		
	}
		
		
	//UE_LOG(LogTemp, Warning, TEXT("FireHit"));
}


void AFireballProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	
	
	//UE_LOG(LogTemp, Warning, TEXT("FireHit"));
	Explode();
	Destroy();
	GetWorld()->GetGameInstance()->GetSubsystem<UMasterMindInstancedSubsystem>()->OnSoundMade.Broadcast(Hit.Location);
	// Only add impulse and destroy projectile if we hit a physics
	/*if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Explode();
		Destroy();
	}*/
}

void AFireballProjectile::DisablePlayerCollision(FHitResult& Hit)
{
	if(Hit.GetActor()->Tags.Contains("Destructible"))
	{
		Hit.GetComponent()->SetCollisionProfileName("IgnorePlayerTest", true);
		Hit.GetComponent()->SetCollisionObjectType(ECC_Destructible);
	}
}


