// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "SPMCharacter.h"
#include "SPMProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
//#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FireballProjectile.h"
#include "ElectricProjectile.h"
#include "DrawDebugHelpers.h"
#include "EnemyBaseClass.h"
#include "GameFramework/Controller.h"
#include "UObject/UObjectBase.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	ElectricOffset = FVector(40.0f, 0.0f, 15.0f);
	
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			World->SpawnActor<ASPMProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::ShootFireball()
{
	
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (FireballClass != nullptr)
	{
		if(ManaComponent->GetMana() < ManaCost)
		{
			return;
		}
		ManaComponent->DecreaseMana(ManaCost);
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			if(PlayerController->IsInputKeyDown(EKeys::R))
			{
				if(BlueFireballClass != nullptr)
				{
					World->SpawnActor<AFireballProjectile>(FireballClass, SpawnLocation, SpawnRotation, ActorSpawnParams);	
				}
			}
			else
			{
				World->SpawnActor<AFireballProjectile>(FireballClass, SpawnLocation, SpawnRotation, ActorSpawnParams);	
			}
			
		}
	}
	
	// Try and play the sound if specified
	if (FireballSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireballSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}


void UTP_WeaponComponent::ShootElectricity()
{
	//UE_LOG(LogTemp, Warning, TEXT("Shoot Electric"));
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ElectricityClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			/*APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			World->SpawnActor<AElectricProjectile>(ElectricityClass, SpawnLocation, SpawnRotation, ActorSpawnParams);*/

			if(ManaComponent->GetMana() < ElectricManaCost)
			{
				return;
			}
			ManaComponent->DecreaseMana(ElectricManaCost);
			FHitResult OutHit;

			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			FVector Start = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(FVector(ElectricOffset));
			FVector CameraForwardVector = PlayerController->PlayerCameraManager->GetActorForwardVector();
			//FVector End = Start + (GetForwardVector() * 5000.0f);
			FVector End = Start + (CameraForwardVector * 5000.0f);

			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this->GetOwner());

			//spawn electric effect
			DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 5, 0 ,1);
			//UE_LOG(LogTemp, Warning, TEXT("%s"), GetChildComponent(0));
			
			/*if(PlayNiagara)
			{
				UNiagaraComponent* ElectricEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(ElectricNiagara, GetChildComponent(0), NAME_None, FVector(100.f), FRotator(0.f, 80.f,0.f), EAttachLocation::Type::KeepRelativeOffset, true);
				PlayNiagara = false;
			}*/

			bool IsHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
			//UE_LOG(LogTemp, Warning, TEXT("%s"), OutHit.GetComponent()->GetClass()->IsChildOf(AEnemyBaseClass::StaticClass()))
			
			if(IsHit && Cast<AEnemyBaseClass>(OutHit.GetActor()))
			{
				//UE_LOG(LogTemp, Warning, TEXT("%p"), UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetInstigatorController());
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerController->GetName());
				//UE_LOG(LogTemp, Warning, TEXT("%p"), PlayerController->GetInstigatorController());
				//deal damage
				//UGameplayStatics::ApplyDamage(OutHit.GetActor(), DamageComponent->GetDamage(),UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetInstigatorController() , UGameplayStatics::GetPlayerCharacter(GetWorld(),0), DamageComponent->GetDamageType());
				//UGameplayStatics::ApplyDamage(OutHit.GetActor(), DamageComponent->GetDamage(),PlayerController,PlayerController , DamageComponent->GetDamageType());
				//UE_LOG(LogTemp, Display, TEXT("ElectricHit"));
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *OutHit.GetActor()->GetName());
				/*for(int32 i = 0; i < 3; i++)
				{
					FHitResult BounceHit;

					FVector BounceStart = OutHit.GetActor()->GetActorLocation();
					FVector BounceEnd = BounceStart + (OutHit.GetActor()->GetActorForwardVector() * 5000.0f);

					FCollisionQueryParams CollisionBounceParams;
					CollisionBounceParams.AddIgnoredActor(OutHit.GetActor());
					DrawDebugLine(GetWorld(), BounceStart, BounceEnd, FColor::Blue, false, 5, 0 ,1);

					bool IsBounceHit = GetWorld()->LineTraceSingleByChannel(BounceHit, BounceStart, BounceEnd, ECC_Visibility, CollisionBounceParams);
					if(IsBounceHit)
					{
						UE_LOG(LogTemp, Warning, TEXT("%s"), *BounceHit.GetActor()->GetName());
					}

				}*/

				TArray<FHitResult> SpreadHits;

				FVector HitLocation = OutHit.GetActor()->GetActorLocation();
		
				FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(ElectricRadius);
				//DrawDebugSphere(GetWorld(), HitLocation, CollisionSphere.GetSphereRadius(), 25, FColor::Red, true);
				bool isSpreadHit = GetWorld()->SweepMultiByChannel(SpreadHits, HitLocation, HitLocation,
					FQuat::Identity, ECC_WorldStatic, CollisionSphere);
				
				for(FHitResult& Hit : SpreadHits)
				{
					if(isSpreadHit && Hit.GetActor() != nullptr && Cast<AEnemyBaseClass>(Hit.GetActor()))
					{
						//Deal Damage to other enemies
						DrawDebugLine(GetWorld(), HitLocation, Hit.GetActor()->GetActorLocation(), FColor::Red, false, 5,0,1);
						
						//DisablePlayerCollision(Hit);
						//UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());
						//UE_LOG(LogTemp, Warning, TEXT("%s"), *MeshComponent->GetName());
						
						//UGameplayStatics::ApplyDamage(Hit.GetActor(), DamageComponent->GetDamage(), this->GetInstigatorController(), this, DamageComponent->GetDamageType());
						//if(MeshComponent /*&& Cast<AEnemyBaseClass>(Hit.GetActor())*/)
						/*{
							//MeshComponent->AddRadialImpulse(HitLocation, ExplosiveRadius, ExplosiveImpulseStrength, RIF_Constant, true);
							DrawDebugLine(GetWorld(), HitLocation, Hit.GetActor()->GetActorLocation(), FColor::Red, false, 5,0,1);
						}*/
						//DestroyWithFireball();	
					}
				}
			}
			
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		//UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::AttachWeapon(ASPMCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no rifle yet
	if (Character == nullptr || Character->GetHasRifle())
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	
	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
			
			EnhancedInputComponent->BindAction(ShootFireballAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::ShootFireball);

			EnhancedInputComponent->BindAction(ShootBlueFireballAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::ShootFireball);

			EnhancedInputComponent->BindAction(ShootElectricityAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::ShootElectricity);
		}
	}
}

void UTP_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	ManaComponent = UGameplayStatics::GetPlayerCharacter(this, 0)->GetComponentByClass<UManaComponent>();
}


void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}