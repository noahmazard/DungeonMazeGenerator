// Fill out your copyright notice in the Description page of Project Settings.


#include "Labyrinth.h"

// Sets default values
ALabyrinth::ALabyrinth()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ALabyrinth::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//Set random seed
	FMath::SRandInit(Seed);
	
	if (WallMeshClass)
	{
		for (int x = 0; x < LabSize.X; x++)
		{
			for (int y = 0; y < LabSize.Y; y++)
			{
				//random
				if (FMath::RandBool())
				{
					continue;
				}
				
				CreateWall(x, y, North);
				CreateWall(x, y, South);
				CreateWall(x, y, East);
				CreateWall(x, y, West);
			}
		}
	}
}

// Called when the game starts or when spawned
void ALabyrinth::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALabyrinth::CreateWall(int x, int y, EDirection Direction)
{
	FVector2D Offset = FVector2D(0, 0);
	FRotator Rotation = FRotator(0, 0, 0);
	switch (Direction)
	{
	case North:
		Offset = FVector2D(0 + (bMoveHalfTile ? 0.5 : 0), 0.5);
		Rotation = FRotator(0, 180, 0);
		break;
	case South:
		Offset = FVector2D(0 - (bMoveHalfTile ? 0.5 : 0), -0.5 );
		Rotation = FRotator(0, 0, 0);
		break;
	case East:
		Offset = FVector2D(0.5, 0 - (bMoveHalfTile ? 0.5 : 0));
		Rotation = FRotator(0, 90, 0);
		break;
	case West:
		Offset = FVector2D(-0.5, 0 + (bMoveHalfTile ? 0.5 : 0));
		Rotation = FRotator(0, -90, 0);
		break;
	default: ;
	}
	
	UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(this);
	Mesh->SetStaticMesh(WallMeshClass);
	Mesh->SetWorldLocation(FVector(TileSize.X * (Offset.X + static_cast<double>(x)), TileSize.Y * (Offset.Y + static_cast<double>(y)), 0));
	Mesh->SetWorldRotation(Rotation);
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->RegisterComponent();
	Mesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	Meshes.Add(Mesh);
}

// Called every frame
void ALabyrinth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

