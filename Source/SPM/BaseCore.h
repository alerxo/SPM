// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCore.generated.h"

class ASpawnPoints;
class AInteractableTerminal;
class USPMGameInstanceSubsystem;
class UHealthComponent;
class USphereComponent;

//Delegate Declaration for FOnCoreActivate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoreActivate);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoreDeactivated);

UCLASS()

class SPM_API ABaseCore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseCore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//Delegate for OnCoreActivate

	FOnCoreActivate OnCoreActivate;
	/*
	UFUNCTION(BlueprintCallable)
	FOnCoreDeactivated OnCoreDeactivated;
	*/
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Setter for bool
	void SetCanBeActivated(bool Value);

	//Getter for USphereComponent
	USphereComponent* GetSphereComponent() const {return SphereComponent;}

	//Function to Damage Core
	
	void DamageCore(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	//HealthComponent
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;

	//MeshComponent
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	//GameInstanceSubSystem for calling delegates
	UPROPERTY()
	USPMGameInstanceSubsystem* GameInstanceSubsystem;

	//SphereComponent EditAnywhere
	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ASpawnPoints*> Spawners;

private:
	//Bool if Core is active or not
	bool bCanBeActivated;

	UFUNCTION(BlueprintCallable)
	//Destroy the core
	void DestroyCore();

};
