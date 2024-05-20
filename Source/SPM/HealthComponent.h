// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable)
	FOnDead OnDead;
	
	//Setter float that sets health
	void SetHealth(float Value){ DefaultHealth = Value;}

	//float Property for Health
	UPROPERTY(EditAnywhere)
	float Health = 100;

	//TakeDamage method so Owner takes Damage When hit
	UFUNCTION()
	virtual void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	//Getter for Getting a float that is Health
	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	void RestoreHealth(float HealthToRestore);

	//Getter method for the Health Precentage
	UFUNCTION(BlueprintCallable)
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintCallable)
	void SetHealthNew(float NewHealth);

	UFUNCTION(BlueprintCallable)
	void ActivateTakeDamage();

	UFUNCTION(BlueprintCallable)
	void DisableTakeDamage();

	UFUNCTION(BlueprintCallable)
	float GetDefaultHealth() const;
	
private:
	//Float property for DefaultHealth that is used for removing Healt Points
	float DefaultHealth;
	
};
