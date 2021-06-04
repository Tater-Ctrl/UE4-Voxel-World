#pragma once

#include "CoreMinimal.h"
#include "MultiArray.h"
#include "MapVariables.h"
#include "ChunkManager.h"

/**
* 
*/
class PROCMAP_API ABlockEditor
{
	public:
	ABlockEditor(AChunkManager* Cm);
	AChunkManager* Cm;

	void BreakBlock(FVector WorldPosition, FVector HitNormal, TMap<FVector2D, FMultiArray>* NoiseMap) const;
	void PlaceBlock(FVector WorldPosition, FVector HitNormal, TMap<FVector2D, FMultiArray>* NoiseMap) const;

	void UpdateSurroundingChunkBlocks(FVector2D ChunkID, FVector Position, TMap<FVector2D, FMultiArray>* NoiseMap) const;
	
	~ABlockEditor();
};