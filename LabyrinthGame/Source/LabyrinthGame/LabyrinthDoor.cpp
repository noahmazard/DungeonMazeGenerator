// Fill out your copyright notice in the Description page of Project Settings.


#include "LabyrinthDoor.h"

#include "LabyrinthGameGameMode.h"


// Sets default values
ALabyrinthDoor::ALabyrinthDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ALabyrinthDoor::Interact_Implementation()
{
	IInteractable::Interact_Implementation();

	//Check if the player has a key
	ALabyrinthGameGameMode* gameMode = Cast<ALabyrinthGameGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode)
	{
		if (gameMode->nbKeys >= nbKeysRequired)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You have open the door")));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Not enough keys, You have %d/%d keys"), gameMode->nbKeys, nbKeysRequired));
		}
	}
}

// Called when the game starts or when spawned
void ALabyrinthDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALabyrinthDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

