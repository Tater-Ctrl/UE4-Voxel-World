#include "BlockEditor.h"

ABlockEditor::ABlockEditor(TMap<FVector2D, FMultiArray>* NoiseMap, TMap<FVector2D, AChunk*>* Chunks)
{
	this->NoiseMap = NoiseMap;
	this->Chunks = Chunks;
}

void ABlockEditor::BreakBlock(FVector WorldPosition, FVector HitNormal) const
{
	const FVector2D ChunkID = FVector2D(floor(WorldPosition.X / CHUNK_OFFSET), floor(WorldPosition.Y / CHUNK_OFFSET));
	
	const FVector Position = FVector(floor(WorldPosition.X / BLOCK_SIZE - HitNormal.X / 2) - (ChunkID.X * CHUNK_WIDTH),
										floor(WorldPosition.Y / BLOCK_SIZE - HitNormal.Y / 2) - (ChunkID.Y * CHUNK_WIDTH), 
											floor(WorldPosition.Z / BLOCK_SIZE - HitNormal.Z / 2));
	
	if(NoiseMap->Find(ChunkID))
	{
		if(Position.Z == 0) return;
		if(NoiseMap->operator[](ChunkID).Get(Position) >= 0)
		{
			NoiseMap->operator[](ChunkID).Set(Position.X, Position.Y, Position.Z, 0);

			UpdateSurroundingChunkBlocks(ChunkID, Position);
		
			Chunks->operator[](ChunkID)->UpdateChunkMesh();
		} else
		{
			if(Position.X >= CHUNK_WIDTH)
			{
				NoiseMap->operator[](FVector2D(ChunkID.X + 1, ChunkID.Y)).Set(0, Position.Y, Position.Z, 0);
				UpdateSurroundingChunkBlocks(ChunkID, Position);
				Chunks->operator[](FVector2D(ChunkID.X + 1, ChunkID.Y))->UpdateChunkMesh();
				Chunks->operator[](ChunkID)->UpdateChunkMesh();
			}
			if(Position.X < 0)
			{
				NoiseMap->operator[](FVector2D(ChunkID.X - 1, ChunkID.Y)).Set(CHUNK_WIDTH - 1, Position.Y, Position.Z, 0);
				UpdateSurroundingChunkBlocks(ChunkID, Position);
				Chunks->operator[](FVector2D(ChunkID.X - 1, ChunkID.Y))->UpdateChunkMesh();
				Chunks->operator[](ChunkID)->UpdateChunkMesh();
			}
			if(Position.Y >= CHUNK_WIDTH)
			{
				NoiseMap->operator[](FVector2D(ChunkID.X, ChunkID.Y + 1)).Set(Position.X, 0, Position.Z, 0);
				UpdateSurroundingChunkBlocks(ChunkID, Position);
				Chunks->operator[](FVector2D(ChunkID.X, ChunkID.Y + 1))->UpdateChunkMesh();
				Chunks->operator[](ChunkID)->UpdateChunkMesh();
			}
			if(Position.Y < 0)
			{
				NoiseMap->operator[](FVector2D(ChunkID.X, ChunkID.Y - 1)).Set(Position.X, CHUNK_WIDTH - 1, Position.Z, 0);
				UpdateSurroundingChunkBlocks(ChunkID, Position);
				Chunks->operator[](FVector2D(ChunkID.X, ChunkID.Y - 1))->UpdateChunkMesh();
				Chunks->operator[](ChunkID)->UpdateChunkMesh();
			}
		}
	}
}

void ABlockEditor::PlaceBlock(FVector WorldPosition, FVector HitNormal) const
{
	// Get the Chunk which you are placing a block in
	const FVector2D ChunkID = FVector2D(floor(WorldPosition.X / CHUNK_OFFSET), floor(WorldPosition.Y / CHUNK_OFFSET));

	// Get the position where to place the new block
	const FVector Position = FVector(floor(WorldPosition.X / BLOCK_SIZE + HitNormal.X / 2) - ChunkID.X * CHUNK_WIDTH,
										floor(WorldPosition.Y / BLOCK_SIZE + HitNormal.Y / 2) - ChunkID.Y * CHUNK_WIDTH, 
											floor(WorldPosition.Z / BLOCK_SIZE + HitNormal.Z / 2));
	
	if(NoiseMap->Find(ChunkID))
	{
		if(NoiseMap->operator[](ChunkID).Get(Position) >= 0)
		{
			NoiseMap->operator[](ChunkID).Set(Position.X, Position.Y, Position.Z, 1);
			UpdateSurroundingChunkBlocks(ChunkID, Position);
			Chunks->operator[](ChunkID)->UpdateChunkMesh();
		} else
		{
			if(Position.X >= CHUNK_WIDTH)
			{
				NoiseMap->operator[](FVector2D(ChunkID.X + 1, ChunkID.Y)).Set(0, Position.Y, Position.Z, 1);
				Chunks->operator[](FVector2D(ChunkID.X + 1, ChunkID.Y))->UpdateChunkMesh();
				Chunks->operator[](ChunkID)->UpdateChunkMesh();
			}
			if(Position.X < 0)
			{
				NoiseMap->operator[](FVector2D(ChunkID.X - 1, ChunkID.Y)).Set(CHUNK_WIDTH - 1, Position.Y, Position.Z, 1);
				Chunks->operator[](FVector2D(ChunkID.X - 1, ChunkID.Y))->UpdateChunkMesh();
				Chunks->operator[](ChunkID)->UpdateChunkMesh();
			}
			if(Position.Y >= CHUNK_WIDTH)
			{
				NoiseMap->operator[](FVector2D(ChunkID.X, ChunkID.Y + 1)).Set(Position.X, 0, Position.Z, 1);
				Chunks->operator[](FVector2D(ChunkID.X, ChunkID.Y + 1))->UpdateChunkMesh();
				Chunks->operator[](ChunkID)->UpdateChunkMesh();
			}
			if(Position.Y < 0)
			{
				NoiseMap->operator[](FVector2D(ChunkID.X, ChunkID.Y - 1)).Set(Position.X, CHUNK_WIDTH - 1, Position.Z, 1);
				Chunks->operator[](FVector2D(ChunkID.X, ChunkID.Y - 1))->UpdateChunkMesh();
				Chunks->operator[](ChunkID)->UpdateChunkMesh();
			}
		}
	}
}

void ABlockEditor::UpdateSurroundingChunkBlocks(const FVector2D ChunkID, const FVector Position) const
{
	if(NoiseMap->operator[](ChunkID).Get(Position.X + 1, Position.Y, Position.Z) == -1) Chunks->operator[](FVector2D(ChunkID.X + 1, ChunkID.Y))->UpdateChunkMesh();
	if(NoiseMap->operator[](ChunkID).Get(Position.X - 1, Position.Y, Position.Z) == -1) Chunks->operator[](FVector2D(ChunkID.X - 1, ChunkID.Y))->UpdateChunkMesh();
	if(NoiseMap->operator[](ChunkID).Get(Position.X, Position.Y + 1, Position.Z) == -1) Chunks->operator[](FVector2D(ChunkID.X, ChunkID.Y + 1))->UpdateChunkMesh();
	if(NoiseMap->operator[](ChunkID).Get(Position.X, Position.Y - 1, Position.Z) == -1) Chunks->operator[](FVector2D(ChunkID.X, ChunkID.Y - 1))->UpdateChunkMesh();
}

ABlockEditor::~ABlockEditor()
{
}