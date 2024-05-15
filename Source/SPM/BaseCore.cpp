// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCore.h"
#include "SPMGameInstanceSubsystem.h"
#include "HealthComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCore::ABaseCore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	RootComponent = SphereComponent;
	MeshComponent->SetupAttachment(RootComponent);

	//HealthComponent->SetActive(false);
}

// Called when the game starts or when spawned
void ABaseCore::BeginPlay()
{
	Super::BeginPlay();

	//instantiate
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	GameInstanceSubsystem = GameInstance->GetSubsystem<USPMGameInstanceSubsystem>();

	//Add DamageCore Method to delegate OnTakeAnyDamage
	OnTakeAnyDamage.AddDynamic(this, &ABaseCore::DamageCore);
	//SphereComponent->OnComponentHit.AddDynamic(this, &ABaseCore::Test);ff
}

//Damages the core
void ABaseCore::DamageCore(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	//Checks if it can take Damage
	if(HealthComponent->GetHealth() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Core Destroyed"));
		DestroyCore();
		Destroy();
		return;
	}
		UE_LOG(LogTemp, Warning, TEXT("ApplyDamage"));
		HealthComponent->TakeDamage(this, Damage,DamageType, InstigatedBy,this);
}


// Called every frame
void ABaseCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Destroy The Core
void ABaseCore::DestroyCore()
{
	if(GameInstanceSubsystem)
	{
		//Broadcast (Activate) Delegate OnCoreDestroyed
		UE_LOG(LogTemp, Warning, TEXT("DestroyCore GameInstance"));
		GameInstanceSubsystem->OnCoreDestroyed.Broadcast(this);
	}
}

//Set So the core can be shoot
void ABaseCore::SetCanBeActivated(bool Value)
{
	bCanBeActivated = true;
	//HealthComponent->SetHealth(HealthComponent->Health);
}



