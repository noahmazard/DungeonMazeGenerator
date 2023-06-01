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
	// Sets default values for this actor's properties
	ALabyrinth();

	// Called when the game starts or when spawned
	virtual void OnConstruction(const FTransform& Transform) override;

protected:

	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMesh* WallMeshClass;

	UPROPERTY(EditAnywhere, Category = Mesh)
	bool bMoveHalfTile = false;
	
	UPROPERTY(EditAnywhere, Category = Generation)
	int Seed = 0;
	
	UPROPERTY(EditAnywhere, Category = Generation)
	FVector2D LabSize = FVector2D(10, 10);

	UPROPERTY(EditAnywhere, Category = Generation)
	FVector2D TileSize = FVector2D(200, 200);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
private:
	UPROPERTY()
	TArray<UStaticMeshComponent*> Meshes;

	void CreateWall(int x, int y, EDirection Direction);

	MazeGenerator Generator;

	int lastSeed = -1;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
