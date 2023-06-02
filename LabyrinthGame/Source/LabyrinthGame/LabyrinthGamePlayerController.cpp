// Fill out your copyright notice in the Description page of Project Settings.


#include "LabyrinthGamePlayerController.h"
#include "Interactable.h"

ALabyrinthGamePlayerController::ALabyrinthGamePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALabyrinthGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("Interact", IE_Pressed, this, &ALabyrinthGamePlayerController::Interact);
}

void ALabyrinthGamePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Get the camera transform
	FVector CamLoc;
	FRotator CamRot;
	GetPlayerViewPoint(CamLoc, CamRot);

	//Get the end of the line trace
	FVector Direction = CamRot.Vector();
	FVector End = CamLoc + (Direction * 1000.f);
	
	//Multi Sphere trace
	TArray<FHitResult> Hits;
	GetWorld()->SweepMultiByChannel(Hits, CamLoc, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(SphereTraceRadius));

	if (Hits.Num() > 0)
	{
		//Sort the hits by distance
		Hits.Sort([](const FHitResult& A, const FHitResult& B) {
			return A.Distance < B.Distance;
		});

		//Iterate over the hits
		for (const FHitResult& Hit : Hits)
		{
			//Check if the hit actor implements the interactable interface
			Interactable = Cast<IInteractable>(Hit.GetActor());
			break;
		}
	}
	else
	{
		Interactable = nullptr;
	}
}

void ALabyrinthGamePlayerController::Interact()
{
	if (Interactable)
	{
		Interactable->Execute_Interact(Cast<UObject>(Interactable));
	}
}
