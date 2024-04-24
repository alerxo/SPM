// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseClass.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"

// Sets default values
AEnemyBaseClass::AEnemyBaseClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsDead())
	{
		SpawnHealthPickup();
		GetController()->Destroy();
		Destroy();
	}
}

// Called to bind functionality to input
void AEnemyBaseClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AEnemyBaseClass::IsDead() const
{
	UHealthComponent* HealthComponent = GetComponentByClass<UHealthComponent>();
	return HealthComponent->GetHealth() <= 0;
}

void AEnemyBaseClass::SpawnHealthPickup()
{
	if (CheckToSpawnPickup())
	{
		GetWorld()->SpawnActor<AHealthPickup>(HealthPickupClass, GetActorLocation(), GetActorRotation());
	}
}

bool AEnemyBaseClass::CheckToSpawnPickup() const
{
	const UHealthComponent* HealthComponent =  UGameplayStatics::GetPlayerCharacter(this, 0)->GetComponentByClass<UHealthComponent>();

	if (HealthComponent == nullptr)
	{
		return false;
	}
	
	const float RandomNumber = FMath::RandRange(0, 100);

	UE_LOG(LogTemp, Warning, TEXT("%f"), RandomNumber)

	if (RandomNumber <= ChanceToSpawnPickup)
	{
		return true;
	}
	if (HealthComponent->GetHealthPercentage() < LowHealthThreshold)
	{
		if (RandomNumber <= ChanceToSpawnPickupLowHP)
		{
			return true;
		}
	}
	return false;
}

