// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Create Subobjects
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Nesh"));

	//Set up the Hierarchy of the Interactable 
	RootComponent = SphereComponent;
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	//Get InputComponent
	UInputComponent* PlayerInput = UGameplayStatics::GetPlayerController(GetWorld(),0)->InputComponent;
	
	if(PlayerInput)
	{
		//Bind Player Input Action to Interact method
		FInputActionBinding InputActionBinding = PlayerInput->BindAction(TEXT("Interact"), IE_Pressed, this, &AInteractable::Interact);
	}


	SetUpCollision();
	
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//Called When another actor Begins To Overlap with collision
void AInteractable::SphereBeginOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	bIsInteractable = true;
}
//Called When another actor Exit The collision
void AInteractable::SphereEndOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsInteractable = false;
}

//Called When Player Interacts
void AInteractable::Interact()
{
	if(bIsInteractable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact Interactable"));
	}

}

//Set up the collision Delegates
void AInteractable::SetUpCollision()
{
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::SphereBeginOverlaped);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AInteractable::SphereEndOverlaped);
}



/*
void AInteractable::SetWidgetVisability(UUserWidget* WidgetComponent)
{
	WidgetComponent->SetVisibility(ESlateVisibility::Visible);
}
*/

