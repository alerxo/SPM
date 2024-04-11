// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableTerminal.h"

#include "BaseCore.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


void AInteractableTerminal::BeginPlay()
{
	
	AActor::BeginPlay();
	UInputComponent* PlayerInput = UGameplayStatics::GetPlayerController(GetWorld(),0)->InputComponent;
	if(PlayerInput)
	{
		FInputActionBinding IAP = PlayerInput->BindAction(TEXT("Interact"), IE_Pressed, this, &AInteractable::Interact);
	}
	SetUpCollision();
	
}

void AInteractableTerminal::Interact()
{
	if(GetIsInteractable())
	{
		if(Core)
		{
			Core->SetCanBeActivated(true);
			UE_LOG(LogTemp, Warning, TEXT("Interact Terminal"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No Core Selected"));
		}
	}
	
}
