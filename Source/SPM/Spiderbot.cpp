// Fill out your copyright notice in the Description page of Project Settings.


#include "Spiderbot.h"
#include "EnemieEnum.h"
#include "MasterMindInstancedSubsystem.h"
#include "SpiderbotProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpiderbot::ASpiderbot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);

	EnemyType = ESpider;
}

// Called when the game starts or when spawned
void ASpiderbot::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetGameInstance()->GetSubsystem<UMasterMindInstancedSubsystem>()->IncreasEnemyAmount(ESpider);
}

// Called every frame
void ASpiderbot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//rotates AI to where it's aiming
	FRotator ControlRotation = GetControlRotation();
	FRotator ActorRotation = GetActorRotation();

	FRotator NewRotation = GetActorRotation();
	NewRotation.Pitch = ControlRotation.Pitch;

	SetActorRotation(NewRotation);

}

//shoots projectile
void ASpiderbot::Fire(TSubclassOf<ASpiderbotProjectile> ProjectileClass, USoundBase* FiringSound)
{
	UGameplayStatics::PlaySoundAtLocation(this, FiringSound, GetActorLocation(), GetActorRotation(), 1, 1, 0, Attenuation);
	ASpiderbotProjectile* Projectile = GetWorld()->SpawnActor<ASpiderbotProjectile>(
		ProjectileClass,
		ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation());
	if (this != nullptr)
	{
		Projectile->SetOwner(this);
	}
}

// Called to bind functionality to input
void ASpiderbot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}