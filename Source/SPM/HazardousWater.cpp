// Fill out your copyright notice in the Description page of Project Settings.


#include "HazardousWater.h"
#include "DamageComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AHazardousWater::AHazardousWater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Setup Water plane Static mesh component
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	RootComponent = Water;
	
	//Setup DamageComponents
	DamageComponent = CreateDefaultSubobject<UDamageComponent>(TEXT("Damage"));
	
	//Setup Water collision
	WaterCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WaterCollision"));
	WaterCollision->SetupAttachment(RootComponent);



 }

// Called when the game starts or when spawned
void AHazardousWater::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	
	OriginalSpeed = Player->GetCharacterMovement()->MaxWalkSpeed;
	Timer = 0;
}

// Called every frame
void AHazardousWater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Timer when player should take damage
	if(bIsPlayerOnWater)
	{
		if(Timer <= 0)
		{
			DamageActor();
		}
		Timer -= 1 * DeltaTime;
	}
	else
	{
		Timer = 0;
	}
}

void AHazardousWater::DamageActor()
{
	//Apply the Damage to Actor
	UGameplayStatics::ApplyDamage(UGameplayStatics::GetPlayerPawn(GetWorld(), 0), DamageComponent->GetDamage(), this->GetInstigatorController(), this,DamageComponent->GetDamageType());
	Timer = TimeUntilDamage;
}

void AHazardousWater::SetMaxAcceleration(float Amount)
{
	//Change the speed of Player when on the water
	if(Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("SET MAX ACCELERATION"));
		Player->GetCharacterMovement()->MaxWalkSpeed = Amount;
	}
	
}



