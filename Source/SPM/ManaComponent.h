// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ManaComponent.generated.h"

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
	
	//Game Instance class for Delegate
	UPROPERTY()
	class USPMGameInstanceSubsystem* Subsystem;
	
	//Decrease the amount of Mana
	//DECLARE_DELEGATE_OneParam(DelegateName, Param1Type)

	
	UFUNCTION(BlueprintCallable)
	void DecreaseMana(float Amount);


	void RechargeMana(float DeltaTime);

private: 

	bool bCanRecharge;

	//Default Value of Timer on Start
	UPROPERTY(EditAnywhere)
	float DefaultTimer = 2;

	//Timer used for time calculation
	float Timer;
	float Mana;
	
	UPROPERTY(EditAnywhere)
	float DefaultMana = 100; 

	UFUNCTION(BlueprintCallable)
	float GetManaPercent() const;
};
