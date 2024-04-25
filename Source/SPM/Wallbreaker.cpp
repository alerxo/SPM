// Fill out your copyright notice in the Description page of Project Settings.


#include "Wallbreaker.h"

#include "HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AWallbreaker::AWallbreaker()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWallbreaker::BeginPlay()
{
	Super::BeginPlay();

	MaxWalkSpeed = this->GetComponentByClass<UCharacterMovementComponent>()->MaxWalkSpeed;
}

// Called every frame
void AWallbreaker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsDead()) GetController()->Destroy(), Destroy();
}

// Called to bind functionality to input
void AWallbreaker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AWallbreaker::GetAcceptanceRadius()
{
	return AcceptanceRadius;
}

void AWallbreaker::SetAcceptanceRadius(float NewAcceptanceRadius)
{
	AcceptanceRadius = NewAcceptanceRadius;
}

float AWallbreaker::GetMaxWalkSpeed()
{
	return MaxWalkSpeed;
}

bool AWallbreaker::IsDead() const
{
	UHealthComponent* HealthComponent = GetComponentByClass<UHealthComponent>();
	return HealthComponent->GetHealth() <= 0;
}

