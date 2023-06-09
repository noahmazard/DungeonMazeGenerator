// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LabyrinthGameGameMode.generated.h"

UCLASS(minimalapi)
class ALabyrinthGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	int nbKeys = 0;

protected:
	ALabyrinthGameGameMode();

	virtual void RestartPlayer(AController* NewPlayer) override;
};



