// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SPMCharacter.generated.h"

class UManaComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
UCLASS(config=Game)

class ASPMCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY()
	class USPMGameInstanceSubsystem* SubSystem;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true")	)
	USceneComponent* FiringPoint;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	/** Dash count value */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Dash, meta=(AllowPrivateAccess = "true"))
	int DashCount;

	UPROPERTY(EditAnywhere)
	class UManaComponent* ManaComponent;

	UPROPERTY(EditAnywhere)
	class UHealthComponent* HealthComponent;
	/** Dash max count */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Dash, meta=(AllowPrivateAccess = "true"))
	int DashMaxCount = 1;

	/** Dash max count */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Dash, meta=(AllowPrivateAccess = "true"))
	float DashSpeed = 5.0f;

	/** Dash cooldowntime */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Dash, meta=(AllowPrivateAccess = "true"))
	float DashCooldown = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Dash, meta=(AllowPrivateAccess = "true"))
	bool bIsDashing = false;
	
public:
	ASPMCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	//Getter for float
	float GetMana() const;
	
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for dashing input */
	void Dash();
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

