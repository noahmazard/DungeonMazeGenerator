// Fill out your copyright notice in the Description page of Project Settings.


#include "Chest.h"

#include "LabyrinthGameGameMode.h"


// Sets default values
AChest::AChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AChest::Interact_Implementation()
{
	IInteractable::Interact_Implementation();
	if (!isOpen)
	{
		isOpen = true;
		
		//Add a key to the player
		ALabyrinthGameGameMode* gameMode = Cast<ALabyrinthGameGameMode>(GetWorld()->GetAuthGameMode());
		if (gameMode)
		{
			gameMode->nbKeys++;
			OpenChest();
		}
	}
}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

