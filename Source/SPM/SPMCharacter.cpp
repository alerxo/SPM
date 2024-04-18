// Copyright Epic Games, Inc. All Rights Reserved.
#include "SPMCharacter.h"
#include "SPMGameInstanceSubsystem.h"
#include "SPMProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ManaComponent.h"
#include "HealthComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnitConversion.h"
#include "Tasks/Task.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);



//////////////////////////////////////////////////////////////////////////
// ASPMCharacter

ASPMCharacter::ASPMCharacter()
{

	//Create ManaComponent
	ManaComponent = CreateDefaultSubobject<UManaComponent>(TEXT("Mana"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	FiringPoint = CreateDefaultSubobject<USceneComponent>(TEXT("FiringPoint"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void ASPMCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();

	SubSystem = GameInstance->GetSubsystem<USPMGameInstanceSubsystem>();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void ASPMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Dashing
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ASPMCharacter::Dash);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASPMCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASPMCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASPMCharacter::Dash()
{
	if(Controller == nullptr)return;

	// Get the MoveDirection
	FVector MoveDirection = this->GetCharacterMovement()->GetLastInputVector();
	
	// Set the MaxMoveSpeed
	double MaxMoveSpeed = this->GetCharacterMovement()->MaxWalkSpeed;

	//Set the DashDirection
	FVector Dash = DashSpeed * MoveDirection * MaxMoveSpeed;

	//Launch the Character if the dashcount is less than max and the character is falling
	if(DashCount < DashMaxCount && this->GetCharacterMovement()->IsFalling())
	{
		LaunchCharacter(Dash, false, false);
		DashCount++;
	}
}


void ASPMCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
		if(DashCount >= DashMaxCount && !Controller->GetCharacter()->GetCharacterMovement()->IsFalling())
		{
			DashCount = 0;
		}
	}
}

void ASPMCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASPMCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ASPMCharacter::GetHasRifle()
{
	return bHasRifle;
}

float ASPMCharacter::GetMana() const
{
	return  ManaComponent->GetMana();
}
