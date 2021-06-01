// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiArray.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "GreedyMesh.generated.h"

/**
 * 
 */
UCLASS()

class PROCMAP_API AGreedyMesh : public AActor
{
	GENERATED_BODY()
	
public:
	AGreedyMesh();
	~AGreedyMesh();

	//int* CHUNK_WIDTH;
	//int* CHUNK_WIDTH;
	//int* CHUNK_HEIGHT;

	//int BLOCK_SIZE = 60;

	TMap<FVector2D, FMultiArray>* NoiseMap;
	void CreateGreedyChunk(FVector2D ChunkID);

private:
	TArray<FVector> Vertices;
	TArray<int32> Indices;
	TArray<FVector> Normals;
	TArray<FVector2D> Uvs;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	UPROPERTY()
	UProceduralMeshComponent* Mesh;
	FMultiArray* Noise;

	void AddIndices(bool Reverse);
	void AddNormals(const FVector Nor);
	
	void CreateTop(int Z);
	void CreateFront(int Y);
	void CreateBack(int Y);
	void CreateRight(int X);
	void CreateLeft(int X);
};
