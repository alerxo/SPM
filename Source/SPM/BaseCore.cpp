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
}

// Called when the game starts or when spawned
void ABaseCore::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	GameInstanceSubsystem = GameInstance->GetSubsystem<USPMGameInstanceSubsystem>();
	
	
	OnTakeAnyDamage.AddDynamic(this, &ABaseCore::Test);
	//SphereComponent->OnComponentHit.AddDynamic(this, &ABaseCore::Test);ff
}

void ABaseCore::Test(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if(bCanBeActivated && HealthComponent->GetHealth() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Core Destroyed"));
		DestroyCore();
	}else
	{
		HealthComponent->TakeDamage(this, Damage,DamageType, InstigatedBy,this);
	}
}


// Called every frame
void ABaseCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABaseCore::DestroyCore()
{
	if(GameInstanceSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("DestroyCore GameInstance"));
		GameInstanceSubsystem->OnCoreDestroyed.Broadcast(true);
	}
	
	Destroy();

	//call event delegate 
}


