// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Chest.generated.h"

UCLASS()
class LABYRINTHGAME_API AChest : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChest();

	virtual void Interact_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OpenChest();

private:
	bool isOpen = false;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
