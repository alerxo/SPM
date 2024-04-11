// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

#include "SPMGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UInputComponent* PlayerInput = UGameplayStatics::GetPlayerController(GetWorld(),0)->InputComponent;
	if(PlayerInput)
	{
		FInputActionBinding IAP = PlayerInput->BindAction(TEXT("Interact"), IE_Pressed, this, &UInteractableComponent::CallInteraction);
		UE_LOG(LogTemp, Warning, TEXT("Bind Action %s"), *GetOwner()->GetName());
	}
}


// Called every frame
void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UInteractableComponent::CallInteraction()
{
	if(bIsInteractable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Call Interaction"));
		//GameInstanceSubsystem->OnInteractable.Broadcast();
	}
	
}

void UInteractableComponent::BindInteractionAction()
{
	/*
	UInputComponent* PlayerInput = UGameplayStatics::GetPlayerController(GetWorld(),0)->InputComponent;
	if(PlayerInput)
	{
		PlayerInput->BindAction(TEXT("Interact"), IE_Pressed, this, &UInteractableComponent::CallInteraction);
		UE_LOG(LogTemp, Warning, TEXT("Bind Action %s"), *GetOwner()->GetName());
	}
	*/
	bIsInteractable = true;
}

void UInteractableComponent::UnBindInteractionAction()
{
	/*
	UInputComponent* PlayerInput = UGameplayStatics::GetPlayerController(GetWorld(),0)->InputComponent;
	if(PlayerInput)
	{
		PlayerInput->BindAction(TEXT("Interact"), IE_Pressed, this, &UInteractableComponent::CallInteraction);
		UE_LOG(LogTemp, Warning, TEXT("UnBind Action %s"), *GetOwner()->GetName());
	}
	*/
	
	bIsInteractable = false;
}



