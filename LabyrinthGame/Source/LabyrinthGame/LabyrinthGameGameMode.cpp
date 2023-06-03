// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthGameGameMode.h"

#include "Labyrinth.h"
#include "LabyrinthGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ALabyrinthGameGameMode::ALabyrinthGameGameMode()
	: Super()
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

	//Get Actor of class Labyrint
	TArray<AActor*> FoundActors;
	ALabyrinth* Labyrinth=  Cast<ALabyrinth>(UGameplayStatics::GetActorOfClass(GetWorld(), ALabyrinth::StaticClass()));
	
	FTransform SpawnTransform;

	if (Labyrinth)
		SpawnTransform.SetLocation(Labyrinth->EntranceLocation + FVector(0, 0, 100));

	RestartPlayerAtTransform(NewPlayer, SpawnTransform);
}
