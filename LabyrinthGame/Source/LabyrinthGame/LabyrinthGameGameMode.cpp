// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthGameGameMode.h"
#include "LabyrinthGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALabyrinthGameGameMode::ALabyrinthGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
