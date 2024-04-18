// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableTerminal.h"

#include "BaseCore.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void AInteractableTerminal::BeginPlay()
{
	
	AActor::BeginPlay();
	
	//Get player input
	UInputComponent* PlayerInput = UGameplayStatics::GetPlayerController(GetWorld(),0)->InputComponent;
	if(PlayerInput)
	{
		//Bind Action sp when player presses Action button Interact Method is called 
		FInputActionBinding InputActionBinding = PlayerInput->BindAction(TEXT("Interact"), IE_Pressed, this, &AInteractable::Interact);
	}
	SetUpCollision();
	
}
//Called When Player Interacts
void AInteractableTerminal::Interact()
{
	if(GetIsInteractable())
	{
		if(Core)
		{
			//Core is Activated
			Core->SetCanBeActivated(true);
			UE_LOG(LogTemp, Warning, TEXT("Interact Terminal"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No Core Selected"));
		}
	}
	
}
