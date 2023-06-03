// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeGenerator.h"
#include "Labyrinth.generated.h"

UCLASS()
class LABYRINTHGAME_API ALabyrinth : public AActor
{
	GENERATED_BODY()
	
public:	
	ALabyrinth();

	UPROPERTY(BlueprintReadOnly)
	FVector EntranceLocation;

protected:

	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMesh* WallMeshClass;

	UPROPERTY(EditAnywhere, Category = Mesh)
	bool bMoveHalfTile = false;

	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMesh* FloorMeshClass;
	
	UPROPERTY(EditAnywhere, Category = Generation)
	int Seed = 0;
	
	UPROPERTY(EditAnywhere, Category = Generation)
	FVector2D LabSize = FVector2D(10, 10);

	UPROPERTY(EditAnywhere, Category = Mesh)
	FVector2D TileSize = FVector2D(200, 200);

	UPROPERTY(EditAnywhere, Category = Generation)
	int nbChests = 1;

	UPROPERTY(EditAnywhere, Category = Mesh)
	TSubclassOf<AActor> ExitActorClass;

	UPROPERTY(EditAnywhere, Category = Mesh)
	TSubclassOf<AActor> EntranceActorClass;
	
	UPROPERTY(EditAnywhere, Category = Mesh)
	TSubclassOf<AActor> ChestActorClass;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = Generation)
	void GenerateLabyrinth();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = Generation)
	void ClearLabyrinth();

	
private:
	UPROPERTY()
	TArray<USceneComponent*> Components;

	void CreateWall(int x, int y, EDirection Direction);
	void CreateFloor(int x, int y);
	void CreateExit(int x, int y, EDirection Direction, TSubclassOf<AActor> actorClass);
	void CreateChest(int x, int y, EDirection Direction);

	void ComputeWallOffset(EDirection Direction, FVector2D& Offset, FRotator& Rotation);
	void ComputeChestOffset(EDirection Direction, FVector2D& Offset, FRotator& Rotation);

	MazeGenerator Generator;

	int lastSeed = -1;
	FVector2D lastSize = FVector2D(-1, -1);

	int nbGeneratedChests = 0;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
