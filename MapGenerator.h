// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.h"
#include "Kismet/GameplayStatics.h"
#include "SimplexNoise.h"
#include "MapGenerator.generated.h"

DECLARE_DELEGATE_OneParam(D_CreateChunkPool, FVector2D)
DECLARE_DELEGATE_OneParam(D_HideChunkPool, FVector2D)

UCLASS()
class PROCMAP_API AMapGenerator : public AActor
{
	GENERATED_BODY()

private:
	void CreateChunk(FVector2D Pos);
	TQueue<AChunk*, EQueueMode::Mpsc> CreateChunkQueue;
	TQueue<AChunk*, EQueueMode::Mpsc> HideChunkQueue;

	void CreateChunkNoise(FVector2D Pos);
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();

	UPROPERTY(EditAnywhere, Category="Chunk Parameters")
	int chunkLoaded = 32;
	UPROPERTY(EditAnywhere, Category="Chunk Parameters")
	int ChunkX = 16;
	UPROPERTY(EditAnywhere, Category="Chunk Parameters")
	int ChunkY = 16;
	UPROPERTY(EditAnywhere, Category="Chunk Parameters")
	int ChunkZ = 256;

	TMap<FVector2D, FMultiArray> NoiseMap;
	UPROPERTY()
	TMap<FVector2D, AChunk*> Chunks;

	SimplexNoise* Simplex;
	
	ACreateCube* Cube;
	UPROPERTY()
	UMaterialInterface* TextureAtlas;
	UPROPERTY()
	AActor* Player;
	
	FVector2D ChunkPosition;

	UFUNCTION(CallInEditor, Category="Chunk Parameters")
	void InitChunks();
	
	UFUNCTION(CallInEditor, Category="Chunk Parameters")
	void DestroyChunks();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	void DrawChunk(const FVector2D Pos);
	void UpdateChunks(FVector2D Pos);
};