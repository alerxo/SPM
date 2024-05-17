// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	//float property for Damage
	UPROPERTY(EditAnywhere)
	float Damage = 1;

	//DamageType Property


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType;
	
	//Getter Return float of Damage
	float GetDamage() const;
	//Getter Return DamageType
	TSubclassOf<UDamageType> GetDamageType() const;
		
};
