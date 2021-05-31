// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.h"
#include "Kismet/GameplayStatics.h"
#include "SimplexNoise.h"
#include "CaveGenerator.h"
#include "BlockEditor.h"
#include "GreedyMesh.h"
#include "MapVariables.h"
#include "MapGenerator.generated.h"

UCLASS()
class PROCMAP_API AMapGenerator : public AActor
{
	GENERATED_BODY()

private:
	void CreateChunk(FVector2D Pos);
	TQueue<AChunk*, EQueueMode::Mpsc> CreateChunkQueue;
	TQueue<AChunk*, EQueueMode::Mpsc> HideChunkQueue;

	CaveGenerator* CaveGen;

	void CreateChunkNoise(FVector2D Pos);
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();

	UPROPERTY(EditAnywhere, Category="Chunk Parameters")
	int chunkLoaded = 32;
	UPROPERTY(EditAnywhere, Category="Chunk Parameters")
	bool EnableGreedyMesh;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chunk Parameters")
	//int CHUNK_WIDTH = 16;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chunk Parameters")
	//int CHUNK_WIDTH = 16;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chunk Parameters")
	//int CHUNK_HEIGHT = 256;
	UPROPERTY(EditAnywhere, Category="Chunk Parameters")
	UCurveFloat* MapCurve;

	UPROPERTY()
	TMap<FVector2D, FMultiArray> NoiseMap;
	UPROPERTY()
	TMap<FVector2D, AChunk*> Chunks;
	UPROPERTY()
	TMap<FVector2D, AGreedyMesh*> GreedyMesh;

	SimplexNoise* Simplex;
	ABlockEditor* BlockEditor;
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
	
	UFUNCTION(BlueprintCallable, Category="Chunk Editing")
	void BreakBlock(FVector Position, FVector HitNormal);
	UFUNCTION(BlueprintCallable, Category="Chunk Editing")
	void PlaceBlock(FVector Position, FVector HitNormal);

	UFUNCTION(CallInEditor, Category="Greedy Chunk")
	void InitGreedyMesh();
	
	void CreateGreedyMesh(FVector2D ChunkID);

	void EditorDrawChunk(const FVector2D Pos);
	void DrawChunk(const FVector2D Pos);
	void UpdateChunks(FVector2D Pos);
};