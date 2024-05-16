// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ManaComponent.h"
//#include "Camera/CameraComponent.h"
#include "DamageComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "TP_WeaponComponent.generated.h"

class ASPMCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ASPMProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AFireballProjectile> FireballClass;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AFireballProjectile> BlueFireballClass;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AElectricProjectile> ElectricityClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireballSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector ElectricOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ShootFireballAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ShootBlueFireballAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ShootElectricityAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwapToBasicAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwapToFireAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwapToElectricAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBasicActive;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFireActive;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bElectricActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireBallCooldown = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanShootFireBall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	float ManaCost = 10.f;

	/*UPROPERTY(EditAnywhere)
	class UMaterial* BasicMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterial* FireMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterial* ElectricMaterial;*/
	

	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(ASPMCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ShootFireball();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ShootElectricity();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ChangeWeapon();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ShootPressed();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ShootHold();
	

	//UFUNCTION(BlueprintCallable)
	//bool TurnOffElectric();

	//UFUNCTION(BlueprintCallable, Category="Weapon")
	//void SpawnElectricity();
	
protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	virtual void BeginPlay() override;

private:
	/** The Character holding this weapon*/
	ASPMCharacter* Character;

	//UPROPERTY(EditAnywhere)
	//UDamageComponent* DamageComponent;

	UPROPERTY(EditAnywhere)
	UManaComponent* ManaComponent;

	//UPROPERTY(EditAnywhere)
	//UCameraComponent* CameraComponent;
	

	UPROPERTY(EditAnywhere, Category="Weapon")
	float ElectricManaCost = 0.5f;

	UPROPERTY(EditAnywhere, Category="Weapon")
	float ElectricRadius = 500.0f;

	UPROPERTY(EditAnywhere, Category="Weapon")
	UNiagaraSystem* ElectricNiagara;

	bool PlayNiagara = true;
	
};
