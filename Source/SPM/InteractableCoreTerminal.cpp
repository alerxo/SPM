// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableCoreTerminal.h"

#include "SPMGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"


void AInteractableCoreTerminal::BeginPlay()
{
	AActor::BeginPlay();
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	GameInstanceSubsystem = GameInstance->GetSubsystem<USPMGameInstanceSubsystem>();

	if(GameInstanceSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay GameInstance"));
		GameInstanceSubsystem->OnCoreDestroyed.AddDynamic(this, &AInteractableCoreTerminal::SetIsActive);
	}
	UInputComponent* PlayerInput = UGameplayStatics::GetPlayerController(GetWorld(),0)->InputComponent;
	if(PlayerInput)
	{
		FInputActionBinding IAP = PlayerInput->BindAction(TEXT("Interact"), IE_Pressed, this, &AInteractable::Interact);
	}
	
	
	SetUpCollision();
}

void AInteractableCoreTerminal::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Should Interact"));
	if(GetIsInteractable())
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Ainteractable Core Terminal"));
		UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	}
}


void AInteractableCoreTerminal::SetIsActive(bool Value)
{
	bIsActive = Value;
}


