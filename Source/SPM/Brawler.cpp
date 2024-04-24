// Fill out your copyright notice in the Description page of Project Settings.


#include "Brawler.h"

#include "HealthComponent.h"

// Sets default values
ABrawler::ABrawler()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABrawler::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrawler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsDead()) GetController()->Destroy(), Destroy();
}

// Called to bind functionality to input
void ABrawler::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABrawler::bIsDead() const
{
	UHealthComponent* HealthComponent = GetComponentByClass<UHealthComponent>();
	return HealthComponent->GetHealth() <= 0;
}

