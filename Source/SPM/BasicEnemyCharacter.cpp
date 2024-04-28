// Fill out your copyright notice in the Description page of Project Settings.

//https://dev.epicgames.com/documentation/en-us/unreal-engine/behavior-tree-in-unreal-engine---quick-start-guide?application_version=5.0
#include "BasicEnemyCharacter.h"

/*Master Mind - Håller koll på spelare och spawnar fiender

NEED TO WORK
-Master mind AI - Controlls everything
	-knows the player position and creates and estimate for the AI enemies to track 
	-
-Potentially SpawnerScript that the master mind takes controll over
	- Spawn enemies at sertan point (Spawn points)
	- Spawn enemies on event when cores shield is deactivated (While terminal timer ticks down)
	- Spawn enemies distance from player behind(out of view)/the Master mind decide this based how the player plays
	-
-Each enemy cost a sertan amount of tokes 
	- this is so the master mind decide what kinds of things to spawn
	-
*/


// Sets default values
ABasicEnemyCharacter::ABasicEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasicEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

