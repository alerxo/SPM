// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "MyRadialForceComponent.generated.h"

/**
 * 
 */
UCLASS(hidecategories = (Object, Mobility, LOD, Physics), ClassGroup = Physics, showcategories = Trigger, meta = (BlueprintSpawnableComponent))
class SPM_API UMyRadialForceComponent : public URadialForceComponent
{
	GENERATED_UCLASS_BODY()
private:

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void BeginPlay() override;
	//~ End UActorComponent Interface.

	//~ Begin UObject Interface.
	virtual void PostLoad() override;

	virtual void FireImpulse() override;
public:
	UPROPERTY(interp, EditAnywhere, BlueprintReadWrite, Category=RadialForceComponent)
	float Height = 100;

};
