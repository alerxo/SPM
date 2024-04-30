// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class UBoxComponent;
class UUserWidget;
class USphereComponent;
UCLASS()

//NOTES FOR THE CLASS SYSTEM 
//CLASSES
//Default Terminal class - Interaction - 
// |							|
//Terminal class activate core// 2 Terminal class load level


//Core class that activates Terminal



//DESCRIPTION
//Terminal for core activation so it can take damage

//Core if destroyed it activates 2 Terminal

//2 Terminal for next level
//is active when core is destroyed

// Called when the game starts
class SPM_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//Called When Player Interacts
	virtual void Interact();

	//Set up the collision Delegates
	void SetUpCollision();

	//Called When another actor Begins To Overlap with collision
	UFUNCTION()
	virtual void SphereBeginOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	//Called When another actor Exit The collision
	UFUNCTION()
	virtual void SphereEndOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	//SphereComponent for collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* SphereComponent;

	//MeshComponent for the Interactables
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	//Getter for Interactable can be interacted with
	bool GetIsInteractable(){return bIsInteractable;}

	//Setter for Interactable can be interacted with
	void SetIsInteractable(bool Value){bIsInteractable = Value;}
private:

	//bool property for IsInteractable
	bool bIsInteractable;
};
