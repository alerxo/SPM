// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"


class USPMGameInstanceSubsystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	bool bIsInteractable;
	
	UPROPERTY()
	USPMGameInstanceSubsystem*  GameInstanceSubsystem;
	
	UFUNCTION(BlueprintCallable)
	void BindInteractionAction();

	UFUNCTION(BlueprintCallable)
	void UnBindInteractionAction();
	
	void CallInteraction();
};
