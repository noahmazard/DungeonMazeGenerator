// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/PlayerController.h"
#include "LabyrinthGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTHGAME_API ALabyrinthGamePlayerController : public APlayerController
{
	GENERATED_BODY()

	ALabyrinthGamePlayerController();
	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditAnywhere)
	float Reach = 1000.f;

	UPROPERTY(EditAnywhere)
	float SphereTraceRadius = 25.f;
	
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	bool canInteract() const { return Interactable != nullptr; }

private:
	void Interact();
	
	IInteractable* Interactable = nullptr;
};
