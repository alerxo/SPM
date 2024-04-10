// Fill out your copyright notice in the Description page of Project Settings.


#include "Spiderbot.h"

#include "SpiderbotProjectile.h"

// Sets default values
ASpiderbot::ASpiderbot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASpiderbot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpiderbot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator ControlRotation = GetControlRotation();
	FRotator ActorRotation = GetActorRotation();

	FRotator NewRotation = GetActorRotation();
	NewRotation.Pitch = ControlRotation.Pitch;

	SetActorRotation(NewRotation);
}

void ASpiderbot::Fire()
{
	ASpiderbotProjectile* Projectile = GetWorld()->SpawnActor<ASpiderbotProjectile>(
		ProjectileClass,
		ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation());
	Projectile->SetOwner(this);
}

// Called to bind functionality to input
void ASpiderbot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

