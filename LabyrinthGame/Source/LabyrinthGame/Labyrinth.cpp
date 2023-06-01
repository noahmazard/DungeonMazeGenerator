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
	if (!WallMeshClass) return;

	if (Seed != lastSeed || LabSize != lastSize)
	{
		//Regenerate maze
		Generator.SetSeed(Seed);
		Generator.SetSize(LabSize.X, LabSize.Y);
		Generator.Generate();
		lastSeed = Seed;
		lastSize = LabSize;
	}
	
	//Generate walls from maze cells
	for (int x = 0; x < LabSize.X; x++)
	{
		for (int y = 0; y < LabSize.Y; y++)
		{
			const MazeCell* Cell = Generator.GetCell(x,y);
			if (!Cell) continue;

			//Only create walls in north and east direction
			if (Cell->HasWall(North))
				CreateWall(x, y, North);
			
			if (Cell->HasWall(East))
				CreateWall(x, y, East);
		}
	}

	//Create outer walls
	for (int x = 0; x < static_cast<int>(LabSize.X); x++)
	{
		CreateWall(x, 0, West);
	}
	for (int y = 0; y < static_cast<int>(LabSize.Y); y++)
	{
		CreateWall(0, y, South);
	}

	//Create floor
	CreateFloor(LabSize.X, LabSize.Y);
}

// Called when the game starts or when spawned
void ALabyrinth::BeginPlay()
{
	Super::BeginPlay();
}

void ALabyrinth::CreateWall(int x, int y, EDirection Direction)
{
	if (!WallMeshClass) return;

	//Compute offset from tile center and rotation for the walls
	FVector2D Offset = FVector2D(0, 0);
	FRotator Rotation = FRotator(0, 0, 0);
	switch (Direction)
	{
	case East:
		Offset = FVector2D(0 + (bMoveHalfTile ? 0.5 : 0), 0.5);
		Rotation = FRotator(0, 180, 0);
		break;
	case West:
		Offset = FVector2D(0 - (bMoveHalfTile ? 0.5 : 0), -0.5);
		Rotation = FRotator(0, 0, 0);
		break;
	case North:
		Offset = FVector2D(0.5, 0 - (bMoveHalfTile ? 0.5 : 0));
		Rotation = FRotator(0, 90, 0);
		break;
	case South:
		Offset = FVector2D(-0.5, 0 + (bMoveHalfTile ? 0.5 : 0));
		Rotation = FRotator(0, -90, 0);
		break;
	default: ;
	}

	//Create wall mesh component
	UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(this);
	Mesh->SetStaticMesh(WallMeshClass);
	Mesh->SetWorldLocation(FVector(TileSize.X * (Offset.X + static_cast<double>(x)),
	                               TileSize.Y * (Offset.Y + static_cast<double>(y)), 0));
	Mesh->SetWorldRotation(Rotation);
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->RegisterComponent();
	Mesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	Meshes.Add(Mesh);
}

void ALabyrinth::CreateFloor(int x, int y)
{
	if (!FloorMeshClass) return;
	
	UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(this);
	Mesh->SetStaticMesh(FloorMeshClass);
	Mesh->SetWorldLocation(FVector(- 0.5 * TileSize.X , - 0.5 * TileSize.Y, 0));
	Mesh->SetWorldScale3D(FVector(x,y,1));
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
