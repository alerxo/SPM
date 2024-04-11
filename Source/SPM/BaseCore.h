// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCore.generated.h"

class USPMGameInstanceSubsystem;
class UHealthComponent;
class USphereComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoreActivate);
UCLASS()

//When Destroyed Activate terminal access
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

	FOnCoreActivate OnCoreActivate;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetCanBeActivated(bool Value){bCanBeActivated = Value;}

	USphereComponent* GetSphereComponent() const {return SphereComponent;}
	
	UFUNCTION()
	void Test(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UPROPERTY(EditAnywhere)
	UHealthComponent* HealthComponent;


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY()
	USPMGameInstanceSubsystem* GameInstanceSubsystem;
private:
	bool bCanBeActivated;

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComponent;

	void DestroyCore();

};
