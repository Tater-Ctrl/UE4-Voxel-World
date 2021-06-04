// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeCreation.h"
#include "Components/RuntimeMeshComponentStatic.h"
#include "MultiArray.h"
#include "ProceduralMeshComponent.h"
#include "ChunkMesh.h"
#include "ChunkManager.generated.h"

UCLASS()
class PROCMAP_API AChunkManager : public AActor
{
	GENERATED_BODY()

	private:
	//TArray<FProcMeshTangent> Tangents;
	TArray<FRuntimeMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;

	UPROPERTY()
	USceneComponent* RootComp;

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
	TMap<FVector2D, AChunkMesh*> ChunkMap;

	UPROPERTY()
	UMaterialInterface* TextureAtlas;

	void SetBlockType(int Sum);
	void UpdateMesh(FVector2D ChunkID);

	void DrawMesh(FVector2D ChunkID);
	void BuildMesh(FVector2D ChunkID);

	void BreakBlock(const FVector WorldPosition, const FVector HitNormal);
	void PlaceBlock(const FVector WorldPosition, const FVector HitNormal);
	void UpdateSurroundingChunkBlocks(const FVector2D ChunkID, const FVector Position);
};
