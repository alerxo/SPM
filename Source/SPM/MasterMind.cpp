// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterMind.h"

// Sets default values
AMasterMind::AMasterMind()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMasterMind::BeginPlay()
{
	Super::BeginPlay();


	OnSpawnOnCore.AddDynamic(this, &AMasterMind::print);
}

// Called every frame
void AMasterMind::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMasterMind::print(int token)
{
	UE_LOG(LogTemp, Warning, TEXT("Call Event OnSpawnOnCore %i" ), token)
}


void AMasterMind::SendInfo(AEnemyBaseClass* Sender, FVector Info)
{
	//OnSentInfo.Broadcast(Sender, Info);
}

void AMasterMind::ReportToOthers(FVector info)
{
	
}



