// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderbotProjectile.h"

#include "DamageComponent.h"
#include "EnemyBaseClass.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpiderbotProjectile::ASpiderbotProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	RootComponent = StaticMeshComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void ASpiderbotProjectile::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ASpiderbotProjectile::OnHit);
}

// Called every frame
void ASpiderbotProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpiderbotProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpuls, const FHitResult& Hit)
{
	AController* MyOwnerInstigator = GetOwner()->GetInstigatorController();
	UDamageComponent* DamageComponent = GetOwner()->GetComponentByClass<UDamageComponent>();
	if (Cast<AEnemyBaseClass>(OtherActor) == nullptr)
	{
		if (OtherActor && OtherActor != this && OtherActor != GetOwner())
		{
			UGameplayStatics::ApplyDamage(OtherActor, DamageComponent->GetDamage(), MyOwnerInstigator, this, DamageComponent->GetDamageType());
		}
	}
	
	Destroy();
}

