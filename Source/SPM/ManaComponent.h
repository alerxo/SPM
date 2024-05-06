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
	UFUNCTION(BlueprintCallable)
	float GetMana() const {return  Mana;}

	UFUNCTION(BlueprintCallable)
	float GetElectricMana() const{return ElectricMana;}
	
	//Game Instance class for Delegate
	UPROPERTY()
	class USPMGameInstanceSubsystem* Subsystem;
	
	//Decrease the amount of Mana
	//DECLARE_DELEGATE_OneParam(DelegateName, Param1Type)

	//Method to Decrease Mana 
	UFUNCTION(BlueprintCallable)
	void DecreaseMana(float Amount);

	//Method to Recharge Mana
	void RechargeMana(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void DecreaseElectricMana(float Amount);

	void RechargeElectricMana(float DeltaTime);

private: 

	//bCanRecharge
	bool bCanRecharge;
	bool bElectricCanRecharge;

	//Default Value of Timer on Start
	UPROPERTY(EditAnywhere)
	float DefaultTimer = 2;

	//Timer used for time calculation
	float Timer;
	float ElectricTimer;
	//float for mana
	float Mana;
	float ElectricMana;

	//Default mana flaot
	UPROPERTY(EditAnywhere)
	float DefaultMana = 100; 

	//Getter for Mana Percentage
	UFUNCTION(BlueprintCallable)
	float GetManaPercent() const;

	UFUNCTION(BlueprintCallable)
	float GetElectricManaPercent() const;
};
