// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseClass.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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
	
	if (IsDead_Implementation())
	{
		if (DeathNiagara != nullptr)
		{
			GetWorld()->SpawnActor<AActor>(DeathNiagara, GetActorLocation(), GetActorRotation());
		}
		SpawnHealthPickup_Implementation();
		GetController()->Destroy();
		Destroy();
	}
}

// Called to bind functionality to input
void AEnemyBaseClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AEnemyBaseClass::IsDead_Implementation() const
{
	UHealthComponent* HealthComponent = GetComponentByClass<UHealthComponent>();
	return HealthComponent->GetHealth() <= 0;
}

bool AEnemyBaseClass::CheckToSpawnPickup_Implementation() const
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

void AEnemyBaseClass::SpawnHealthPickup_Implementation()
{
	if (CheckToSpawnPickup_Implementation())
	{
		GetWorld()->SpawnActor<AHealthPickup>(HealthPickupClass, GetActorLocation(), FRotator::ZeroRotator);
	}
}