// Fill out your copyright notice in the Description page of Project Settings.

#include "Labyrinth.h"

#include "LabyrinthDoor.h"

#define CREATION_METHOD EComponentCreationMethod::Native

// Sets default values
ALabyrinth::ALabyrinth()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create root component
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetWorldLocation(FVector(0, 0, 0));
	//Root->SetupAttachment(RootComponent);
	Root->RegisterComponent();
	RootComponent = Root;
}

// Called when the game starts or when spawned
void ALabyrinth::BeginPlay()
{
	Super::BeginPlay();
}

void ALabyrinth::GenerateLabyrinth()
{
	ClearLabyrinth();

	nbGeneratedChests = 0;

	//Regenerate maze
	Generator.SetSeed(Seed);
	Generator.SetSize(LabSize.X, LabSize.Y);
	Generator.Generate();
	lastSeed = Seed;
	lastSize = LabSize;

	//Create chests
	for (int i = 0; i < nbChests; i++)
	{
		int direction = 0;
		const MazeCell* Cell = Generator.GetChestCell(i, direction);
		if (!Cell) break;
		CreateChest(Cell->x, Cell->y, static_cast<EDirection>(direction));
		nbGeneratedChests++;
	}

	//Generate walls from maze cells
	for (int x = 0; x < LabSize.X; x++)
	{
		for (int y = 0; y < LabSize.Y; y++)
		{
			const MazeCell* Cell = Generator.GetCell(x, y);
			if (!Cell) continue;

			//Only create walls in north and east direction
			if (Cell->HasWall(North) && x != LabSize.X - 1)
				CreateWall(x, y, North);

			if (Cell->HasWall(East) && y != LabSize.Y - 1)
				CreateWall(x, y, East);

			//Create floor
			CreateFloor(x, y);
		}
	}

	//Create exit
	int ExitDirection = 0;
	const MazeCell* ExitCell = Generator.GetExitCell(ExitDirection);
	if (ExitCell)
	{
		if (ExitDirection & North) ExitDirection = North;
		else if (ExitDirection & South) ExitDirection = South;
		else if (ExitDirection & East) ExitDirection = East;
		else if (ExitDirection & West) ExitDirection = West;

		CreateExit(ExitCell->x, ExitCell->y, ExitDirection, ExitActorClass);
	}

	//Create entrance
	int EntranceDirection = 0;
	const MazeCell* EntranceCell = Generator.GetEntranceCell(EntranceDirection);
	if (EntranceCell)
	{
		if (EntranceDirection & North) EntranceDirection = North;
		else if (EntranceDirection & South) EntranceDirection = South;
		else if (EntranceDirection & East) EntranceDirection = East;
		else if (EntranceDirection & West) EntranceDirection = West;
		
		UChildActorComponent* comp = CreateExit(EntranceCell->x, EntranceCell->y, EntranceDirection, EntranceActorClass);
		EntranceComponent = Cast<UChildActorComponent>(comp);
	}


	//Create outer walls
	for (int x = 0; x < static_cast<int>(LabSize.X); x++)
	{
		//if not entrance and entrance direction or exit and exit direction
		if (!((x == EntranceCell->x && EntranceDirection & West) || (x == ExitCell->x && ExitDirection & West)))
			CreateWall(x, 0, West);

		if (!((x == EntranceCell->x && EntranceDirection & East) || (x == ExitCell->x && ExitDirection & East)))
			CreateWall(x, LabSize.Y - 1, East);
	}
	for (int y = 0; y < static_cast<int>(LabSize.Y); y++)
	{
		if (!((y == EntranceCell->y && EntranceDirection & South) || (y == ExitCell->y && ExitDirection & South)))
			CreateWall(0, y, South);
		if (!((y == EntranceCell->y && EntranceDirection & North) || (y == ExitCell->y && ExitDirection & North)))
			CreateWall(LabSize.X - 1, y, North);
	}
}

void ALabyrinth::SetSeed(int _seed)
{
	Seed = _seed;
}

void ALabyrinth::ClearLabyrinth()
{
	//Destroy all actors
	for (auto comp : Components)
	{
		if (comp)
			comp->DestroyComponent();
	}
	Components.Empty();

	EntranceComponent = nullptr;
}

void ALabyrinth::ComputeWallOffset(int Direction, FVector2D& Offset, FRotator& Rotation) const
{
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
}

void ALabyrinth::ComputeChestOffset(EDirection Direction, FVector2D& Offset, FRotator& Rotation) const
{
	switch (Direction)
	{
	case East:
		Offset = FVector2D(0, -0.25);
		Rotation = FRotator(0, 90, 0);
		break;
	case West:
		Offset = FVector2D(0, 0.25);
		Rotation = FRotator(0, -90, 0);
		break;
	case North:
		Offset = FVector2D(-0.25, 0);
		Rotation = FRotator(0, 0, 0);
		break;
	case South:
		Offset = FVector2D(0.25, 0);
		Rotation = FRotator(0, 180, 0);
		break;
	default: ;
	}
}

UStaticMeshComponent* ALabyrinth::CreateWall(int x, int y, EDirection Direction)
{
	if (!WallMeshClass) return nullptr;

	//Compute offset from tile center and rotation for the walls
	FVector2D Offset;
	FRotator Rotation;
	ComputeWallOffset(Direction, Offset, Rotation);

	//Create wall mesh component
	UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(this);
	Mesh->SetStaticMesh(WallMeshClass);
	Mesh->SetWorldLocation(FVector(TileSize.X * (Offset.X + static_cast<double>(x)),
	                               TileSize.Y * (Offset.Y + static_cast<double>(y)), 0));
	Mesh->SetWorldRotation(Rotation);
	Mesh->SetupAttachment(RootComponent);
	Mesh->CreationMethod = CREATION_METHOD;
	Mesh->RegisterComponent();
	Components.Add(Mesh);

	return Mesh;
}

UStaticMeshComponent* ALabyrinth::CreateFloor(int x, int y)
{
	if (!FloorMeshClass) return nullptr;

	UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(this);
	Mesh->SetStaticMesh(FloorMeshClass);
	Mesh->SetWorldLocation(FVector((-0.5 + static_cast<double>(x)) * TileSize.X,
	                               (-0.5 + static_cast<double>(y)) * TileSize.Y, 0));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->CreationMethod = CREATION_METHOD;
	Mesh->RegisterComponent();
	Components.Add(Mesh);

	return Mesh;
}

UChildActorComponent* ALabyrinth::CreateExit(int x, int y, int Direction, TSubclassOf<AActor> actorClass)
{
	if (!actorClass) return nullptr;

	//Compute offset from tile center and rotation for the walls
	FVector2D Offset;
	FRotator Rotation;
	ComputeWallOffset(Direction, Offset, Rotation);

	//Create wall mesh component
	UChildActorComponent* Exit = NewObject<UChildActorComponent>(this);
	Exit->SetChildActorClass(actorClass);
	Exit->SetWorldLocation(FVector(TileSize.X * (Offset.X + static_cast<double>(x)),
	                               TileSize.Y * (Offset.Y + static_cast<double>(y)), 0));
	Exit->SetWorldRotation(Rotation);
	Exit->SetupAttachment(RootComponent);
	Exit->CreationMethod = CREATION_METHOD;
	Exit->RegisterComponent();

	ALabyrinthDoor* Door = Cast<ALabyrinthDoor>(Exit->GetChildActor());
	if (Door)
	{
		Door->nbKeysRequired = nbGeneratedChests;
	}

	Components.Add(Exit);
	return Exit;
}

UChildActorComponent* ALabyrinth::CreateChest(int x, int y, EDirection Direction)
{
	if (!ChestActorClass) return nullptr;

	//Compute offset from tile center and rotation for the walls
	FVector2D Offset;
	FRotator Rotation;
	ComputeChestOffset(Direction, Offset, Rotation);

	//Create wall mesh component
	UChildActorComponent* Chest = NewObject<UChildActorComponent>(this);
	Chest->SetChildActorClass(ChestActorClass);
	Chest->SetWorldLocation(FVector(TileSize.X * (Offset.X + static_cast<double>(x)),
	                                TileSize.Y * (Offset.Y + static_cast<double>(y)), 0));
	Chest->SetWorldRotation(Rotation);
	Chest->SetupAttachment(RootComponent);
	Chest->CreationMethod = CREATION_METHOD;
	Chest->RegisterComponent();

	Components.Add(Chest);
	return Chest;
}


// Called every frame
void ALabyrinth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
