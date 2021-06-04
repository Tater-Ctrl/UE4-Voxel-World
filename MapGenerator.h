// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "SimplexNoise.h"
#include "MultiArray.h"
#include "MapVariables.h"
#include "MapGenerator.generated.h"

class AChunkManager;

UENUM()
enum EChunkMode
{
	GreedyMeshPmc	UMETA(DisplayName = "GreedyMeshPMC"),
	SectionRmc		UMETA(DisplayName = "SectionRMC"),
};

UCLASS()
class PROCMAP_API AMapGenerator : public AActor
{
	GENERATED_BODY()

private:
	//TQueue<AChunk*, EQueueMode::Mpsc> CreateChunkQueue;
	//TQueue<AChunk*, EQueueMode::Mpsc> HideChunkQueue;

	void CreateChunkNoise(FVector2D Pos);
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();

	UPROPERTY(EditAnywhere, Category="Chunk Parameters")
	int chunkLoaded = 32;
	UPROPERTY(EditAnywhere, Category="Chunk Parameters")
	TEnumAsByte<EChunkMode> ChunkMode;
	UPROPERTY(EditAnywhere, Category="Chunk Parameters")
	UCurveFloat* MapCurve;

	UPROPERTY()
	TMap<FVector2D, FMultiArray> NoiseMap;

	SimplexNoise Simplex;
	
	UPROPERTY()
	AChunkManager* ChunkManager;
	
	UPROPERTY()
	UMaterialInterface* TextureAtlas;
	UPROPERTY()
	AActor* Player;
	
	FVector2D ChunkPosition;
	
	UFUNCTION(CallInEditor, Category="Chunk Parameters")
	void DestroyChunks();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category="Chunk Editing")
	void BreakBlock(FVector Position, FVector HitNormal);
	UFUNCTION(BlueprintCallable, Category="Chunk Editing")
	void PlaceBlock(FVector Position, FVector HitNormal);

	UFUNCTION(CallInEditor, Category="Chunk Editing")
	void InitChunkRmc();

	//void EditorDrawChunk(const FVector2D Pos);
	//void DrawChunk(const FVector2D Pos);
	void UpdateChunks(FVector2D Pos);
};