#pragma once

#include "CoreMinimal.h"
#include "MultiArray.h"
#include "MapVariables.h"
#include "Chunk.h"

/**
* 
*/
class PROCMAP_API ABlockEditor
{
	public:
	ABlockEditor(TMap<FVector2D, FMultiArray>* NoiseMap, TMap<FVector2D, AChunk*>* Chunks);

	TMap<FVector2D, FMultiArray>* NoiseMap;
	TMap<FVector2D, AChunk*>* Chunks;

	void BreakBlock(FVector WorldPosition, FVector HitNormal) const;
	void PlaceBlock(FVector WorldPosition, FVector HitNormal) const;

	void UpdateSurroundingChunkBlocks(FVector2D ChunkID, FVector Position) const;
	
	~ABlockEditor();
};