// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeCreation.h"
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

	public:	
	// Sets default values for this actor's properties
	AChunkManager();

	UPROPERTY()
	FCubeCreation Cube;

	UPROPERTY(EditAnywhere, Category="Distance to Player")
	int ChunkDistance = 0;

	UPROPERTY()
	TMap<FVector2D, FMultiArray> NoiseMap;
	UPROPERTY()
	TMap<FVector2D, int> ChunkSection;
	UPROPERTY()
	int ChunkIndex;

	UPROPERTY()
	UMaterialInterface* TextureAtlas;

	void SetBlockType(int Sum);
	void UpdateChunkMesh();

	void DrawChunk(FVector2D ChunkID);
	void CreateChunk(FVector2D ChunkID);
};
