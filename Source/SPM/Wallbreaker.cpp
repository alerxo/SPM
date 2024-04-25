// Fill out your copyright notice in the Description page of Project Settings.


#include "Wallbreaker.h"

#include "HealthComponent.h"

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

bool AWallbreaker::IsDead() const
{
	UHealthComponent* HealthComponent = GetComponentByClass<UHealthComponent>();
	return HealthComponent->GetHealth() <= 0;
}

