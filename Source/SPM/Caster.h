// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Caster.generated.h"
class UInputAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UCaster : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCaster();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	//Method to Create A Ray And apply Damage
	void Cast();

	//Parent InputController
	UPROPERTY()
	UInputComponent* Input;

	//Bool for Debug mode
	UPROPERTY()
	bool DebugMode = false;

public:
	//spell casting cooldown
	UPROPERTY(EditAnywhere) 
	float CoolDown = 0;

	//Damage when Enenmy hit
	UPROPERTY(EditAnywhere)
	float Damage = 1;

	//end distance for the ray
	UPROPERTY(EditAnywhere)
	float EndDistance = 2000;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> DamageType;

	//Spell class
	UPROPERTY(EditAnywhere)
	class UDamageComponent* DamageComponent;
	
	void SetDebug();
};
