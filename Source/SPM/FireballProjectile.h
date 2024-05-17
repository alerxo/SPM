// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageComponent.h"
#include "Field/FieldSystemActor.h"
#include "GameFramework/Actor.h"
#include "FireballProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Config=Game)
class SPM_API AFireballProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	AFireballProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void Explode();

	UFUNCTION()
	void DisablePlayerCollision(FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyWithFireball();

	UFUNCTION(BlueprintImplementableEvent)
	void DealDamage(FVector Location);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Projectile", BlueprintReadOnly, meta=(DisplayName = "ExplosiveRadius", AllowPrivateAccess = "true"))
	float ExplosiveRadius = 300.f;

	UPROPERTY(EditAnywhere, Category="Projectile")
	float ExplosiveImpulseStrength = 2000.f;

	UPROPERTY(EditAnywhere, Category="Projectile")
	UDamageComponent* DamageComponent;
};
