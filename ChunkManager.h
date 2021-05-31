// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreateCube.h"
#include "Components/RuntimeMeshComponentStatic.h"
#include "MultiArray.h"
#include "ChunkManager.generated.h"

UCLASS()
class PROCMAP_API AChunkManager : public AActor
{
	GENERATED_BODY()

	private:
	TArray<FRuntimeMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;

	UPROPERTY()
	USceneComponent* RootComp;
	
	UPROPERTY()
	URuntimeMeshComponentStatic* RuntimeMeshComponent;
	
	int CheckNeighbourChunk(int X, int Y, int Z, int IdX, int IdY);

	public:	
	// Sets default values for this actor's properties
	AChunkManager();

	//UPROPERTY(VisibleAnywhere, Category = "MyProceduralMesh")
	//UProceduralMeshComponent* pm;
	
	ACreateCube* Cube;

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
