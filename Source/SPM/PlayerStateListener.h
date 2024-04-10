// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateListener.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDecreaseManaDelegate, float, Amount);
UCLASS()
class SPM_API APlayerStateListener : public APlayerState
{
	GENERATED_BODY()

public:
	APlayerStateListener(){}

	//DECLARE_EVENT_OneParam( OwningType, EventName, Param1Type )
	UPROPERTY(BlueprintAssignable)
	FOnDecreaseManaDelegate OnDecreaseMana;
	

	//DECLARE_EVENT_OneParam(GamePlayEvent, OnPlayerShootEvent,float);
	//OnPlayerShootEvent& OnPLayerShoot(){return OnPlayerShootEvent;}
};
