// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeCreation.h"
#include "ProceduralMeshComponent.h"
#include "MultiArray.h"
#include "MapVariables.h"
#include "Chunk.generated.h"


UCLASS()
class PROCMAP_API AChunk : public AActor
{
	GENERATED_BODY()

private:
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;
	
	int CheckNeighbourChunk(int X, int Y, int Z, int IdX, int IdY);

public:	
	// Sets default values for this actor's properties
	AChunk();

	UPROPERTY(VisibleAnywhere, Category = "MyProceduralMesh")
	UProceduralMeshComponent* pm;
	
	FCubeCreation* Cube;

	UPROPERTY(EditAnywhere, Category="Chunk ID")
	bool IsActive;
	
	UPROPERTY(EditAnywhere, Category="Chunk ID")
	FVector2D ChunkID;

	UPROPERTY(EditAnywhere, Category="Distance to Player")
	int ChunkDistance = 0;

	void SetBlockType(int Sum);
	void UpdateChunkMesh();

	TMap<FVector2D, FMultiArray>* NoiseMap;

	void DrawChunk();
	void CreateChunk();
};
