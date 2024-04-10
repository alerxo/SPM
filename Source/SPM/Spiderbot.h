// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Spiderbot.generated.h"

UCLASS()
class SPM_API ASpiderbot : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpiderbot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASpiderbotProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* ProjectileSpawnPoint;



	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
