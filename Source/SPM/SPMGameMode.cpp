// Copyright Epic Games, Inc. All Rights Reserved.

#include "SPMGameMode.h"
#include "SPMCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASPMGameMode::ASPMGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
