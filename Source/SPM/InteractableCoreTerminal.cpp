// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableCoreTerminal.h"

#include "BaseCore.h"
#include "InteractableTerminal.h"
#include "SPMGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void AInteractableCoreTerminal::BeginPlay()
{
	AActor::BeginPlay();
	//Create GameInstanceSubSystem to call delegates
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	GameInstanceSubsystem = GameInstance->GetSubsystem<USPMGameInstanceSubsystem>();

	if(GameInstanceSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay GameInstance"));
		//GameInstanceSubsystem->OnCoreDestroyed.AddDynamic(this, &AInteractableCoreTerminal::SetIsActive);
	}
	UInputComponent* PlayerInput = UGameplayStatics::GetPlayerController(GetWorld(),0)->InputComponent;
	if(PlayerInput)
	{
		FInputActionBinding InputActionBinding = PlayerInput->BindAction(TEXT("Interact"), IE_Pressed, this, &AInteractable::Interact);
	}

	if(GameInstanceSubsystem)
	{
		//Dynamically add CheckCOres to OnCoreDestroyed
		GameInstanceSubsystem->OnCoreDestroyed.AddDynamic(this, &AInteractableCoreTerminal::CheckCores);
	}

	
	//Setup The collision
	SetUpCollision();
}
//Called When Player Interacts
void AInteractableCoreTerminal::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Should Interact"));
	//if active and all cores destroyed open level
	if(GetIsActive() && GetIsInteractable())
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Ainteractable Core Terminal"));
		UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	}
}

//Setter Terminal is Active
void AInteractableCoreTerminal::SetIsActive(bool Value)
{
	bIsActive = Value;
}

//function to remove core in array and check it
void AInteractableCoreTerminal::CheckCores(ABaseCore* Core)
{
	if(this == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No CoreTerminal in Interactable"));
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("Check Cores"));
	Cores.Remove(Core);
	//Core->Destroy();
	if(Cores.Num() == 0)
	{
		this->SetIsActive(true);
	}
}





