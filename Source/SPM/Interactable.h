// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class UUserWidget;
class USphereComponent;
UCLASS()

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
	
	
	virtual void Interact();

	void SetUpCollision();
	
	UFUNCTION()
	virtual void SphereBeginOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void SphereEndOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	//Use Trigger
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	bool GetIsInteractable(){return bIsInteractable;}


	void SetIsInteractable(bool Value){bIsInteractable = Value;}
private:

	bool bIsInteractable;
};
