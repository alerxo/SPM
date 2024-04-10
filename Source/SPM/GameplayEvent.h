
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameplayEvent.generated.h"
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDecreaseManaDelegate, float, Amount);
UCLASS()
class SPM_API AGameplayEvent : public AGameState
{
	GENERATED_BODY()
public:
	AGameplayEvent(){}

	//DECLARE_EVENT_OneParam( OwningType, EventName, Param1Type )
	UPROPERTY()
	FOnDecreaseManaDelegate OnDecreaseMana;
	

	//DECLARE_EVENT_OneParam(GamePlayEvent, OnPlayerShootEvent,float);
	//OnPlayerShootEvent& OnPLayerShoot(){return OnPlayerShootEvent;}
};
