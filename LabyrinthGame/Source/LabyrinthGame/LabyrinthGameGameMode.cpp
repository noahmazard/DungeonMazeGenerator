// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthGameGameMode.h"

#include "Labyrinth.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ALabyrinthGameGameMode::ALabyrinthGameGameMode() : Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void ALabyrinthGameGameMode::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}
	
	FTransform SpawnTransform = FTransform::Identity;
	
	if (ALabyrinth* Labyrinth = Cast<ALabyrinth>(UGameplayStatics::GetActorOfClass(GetWorld(), ALabyrinth::StaticClass())))
	{
		// Generate labyrinth if needed
		if (Labyrinth->RandomGenerationAtStart)
		{
			Labyrinth->SetSeed(FMath::Rand());
			Labyrinth->GenerateLabyrinth();
		}

		// Spawn player at entrance
		if (const UChildActorComponent* Entrance = Labyrinth->EntranceComponent)
		{
			const FTransform T = Entrance->GetComponentTransform();
			
			SpawnTransform = FTransform(T.GetRotation(), T.GetLocation() + T.GetRotation().RotateVector(FVector(250, 250, 100)), FVector(1, 1, 1));
		}
	}
	
	RestartPlayerAtTransform(NewPlayer, SpawnTransform);
}
