// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LabyrinthDoor.generated.h"

UCLASS()
class LABYRINTHGAME_API ALabyrinthDoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALabyrinthDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
