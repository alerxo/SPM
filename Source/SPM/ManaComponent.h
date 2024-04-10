// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEvent.h"
#include "Components/ActorComponent.h"
#include "ManaComponent.generated.h"

class AGameplayEvent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UManaComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	
public:	
	// Sets default values for this component's properties
	UManaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Getter for Mana
	float GetMana() const {return  Mana;}

	//Getters and setter DecreaseAmount
	void SetDecreaseAmount(float Amount) { DecreaseAmount = Amount; }
	float GetDecreaseAmount(){return  DecreaseAmount;}

	//TSharedRef<AGameplayEvent> GameplayEvent(new AGameplayEvent());

	//Decrease the amount of Mana
	//DECLARE_DELEGATE_OneParam(DelegateName, Param1Type)

	UFUNCTION()
	void DecreaseMana(float Amount);
private: 

	float Mana;

	UPROPERTY(EditAnywhere)
	float DecreaseAmount = 1;
	UPROPERTY(EditAnywhere)
	float DefaultMana = 100; 

		
};
