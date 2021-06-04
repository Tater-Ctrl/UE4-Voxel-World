// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkManager.h"
#include "MapVariables.h"

// Sets default values
AChunkManager::AChunkManager()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	TextureAtlas = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Texture_Atlas/Atlas_M.Atlas_M"));
}

void AChunkManager::DrawMesh(const FVector2D ChunkID) 
{
	BuildMesh(ChunkID);
	
	const int OffsetX = ChunkID.X * CHUNK_OFFSET;
	const int OffsetY = ChunkID.Y * CHUNK_OFFSET;

	AChunkMesh* NewMesh = GetWorld()->SpawnActor<AChunkMesh>(FVector(OffsetX, OffsetY, 0), FRotator(0, 0, 0));
	ChunkMap.Add(ChunkID, NewMesh);

	ChunkMap[ChunkID]->SetMaterial(TextureAtlas);
	ChunkMap[ChunkID]->CreateMesh(Cube.Vertices, Cube.Triangles, Cube.Normals, Cube.Uvs);

	Cube.ClearMeshData();
}

void AChunkManager::UpdateMesh(FVector2D ChunkID)
{
	BuildMesh(ChunkID);

	ChunkMap[ChunkID]->UpdateMesh(Cube.Vertices, Cube.Triangles, Cube.Normals, Cube.Uvs);
	Cube.ClearMeshData();
}

void AChunkManager::BuildMesh(const FVector2D ChunkID)
{
	double Start = FPlatformTime::Seconds();
	
	for(int X = 0; X < CHUNK_WIDTH; ++X)
	{
		for(int Y = 0; Y < CHUNK_WIDTH; ++Y)
		{
			for(int Z = 0; Z < CHUNK_HEIGHT; ++Z)
			{					
				if(NoiseMap[ChunkID].Get(X, Y, Z) == 0) continue;

				SetBlockType(NoiseMap[ChunkID].Get(X, Y, Z));
				
				// Draw top of block
				if(NoiseMap[ChunkID].Get(X, Y, Z + 1) == BlockType::Air)
				{
					Cube.Top(X, Y, Z );
				}

				// Draw bottom of block
				if(NoiseMap[ChunkID].Get(X, Y, Z - 1) == BlockType::Air)
				{
					Cube.Bottom(X, Y, Z);
				}
				
				// Draw front of block and check neighbour chunks
				if(NoiseMap[ChunkID].Get(X, Y + 1, Z) <= BlockType::Air)
				{
					// Check neighbour chunk if it has a block or not
					if(NoiseMap[ChunkID].Get(X, Y + 1, Z) < BlockType::Air)
					{
						// if neighbour block is Air, display tile face
						if(NoiseMap[FVector2D(ChunkID.X, ChunkID.Y + 1)].Get(X, 0, Z) == BlockType::Air)
						{
							Cube.Front(X, Y, Z);
						}
					} else
					{
						Cube.Front(X, Y, Z);
					}
				}

				// Draw back of block  and check neighbour chunks
				if(NoiseMap[ChunkID].Get(X, Y - 1, Z) <= BlockType::Air)
				{
					// // Check neighbour chunk if it has a block or not
					if(NoiseMap[ChunkID].Get(X, Y - 1, Z) < BlockType::Air)
					{
						// if neighbour chunk block is Air, display tile face
						if(NoiseMap[FVector2D(ChunkID.X, ChunkID.Y - 1)].Get(X, CHUNK_WIDTH - 1, Z) == BlockType::Air)
							Cube.Back(X, Y, Z);
					} else
					{
						Cube.Back(X, Y, Z);
					}
				}
				
				// Draw left of block and check neighbour chunks
				if(NoiseMap[ChunkID].Get(X + 1, Y, Z) <= BlockType::Air)
				{
					// // Check neighbour chunk if it has a block or not
					if(NoiseMap[ChunkID].Get(X + 1, Y, Z) < BlockType::Air)
					{
						// if neighbour chunk block is Air, display tile face
						if(NoiseMap[FVector2D(ChunkID.X + 1, ChunkID.Y)].Get(0, Y, Z) == BlockType::Air)
							Cube.Left(X, Y, Z);
					} else
					{
						Cube.Left(X, Y, Z);
					}
				}
				
				// Draw right of block and check neighbour chunks
				if(NoiseMap[ChunkID].Get(X - 1, Y, Z) <= BlockType::Air)
				{
					// // Check neighbour chunk if it has a block or not
					if(NoiseMap[ChunkID].Get(X - 1, Y, Z) < BlockType::Air)
					{
						// if neighbour chunk block is Air, display tile face
						if(NoiseMap[FVector2D(ChunkID.X - 1, ChunkID.Y)].Get(CHUNK_WIDTH - 1, Y, Z) == BlockType::Air)
							Cube.Right(X, Y, Z);
					} else
					{
						// if neighbour chunk block is Air, display tile face
						Cube.Right(X, Y, Z);
					}
				}
			}
		}
	}

	double End = FPlatformTime::Seconds();
	
	//UE_LOG(LogTemp, Warning, TEXT("%f ms"), (End-Start) * 1000);
}

void AChunkManager::BreakBlock(const FVector WorldPosition, const FVector HitNormal)
{
	const FVector2D ChunkID = FVector2D(floor(WorldPosition.X / CHUNK_OFFSET), floor(WorldPosition.Y / CHUNK_OFFSET));
	
	const FVector Position = FVector(floor(WorldPosition.X / BLOCK_SIZE - HitNormal.X / 2) - (ChunkID.X * CHUNK_WIDTH),
										floor(WorldPosition.Y / BLOCK_SIZE - HitNormal.Y / 2) - (ChunkID.Y * CHUNK_WIDTH), 
											floor(WorldPosition.Z / BLOCK_SIZE - HitNormal.Z / 2));
	
	if(NoiseMap.Find(ChunkID))
	{
		if(Position.Z == 0) return;
		
		if(NoiseMap[ChunkID].Get(Position) >= 0)
		{
			NoiseMap[ChunkID].Set(Position.X, Position.Y, Position.Z, 0);
			UpdateSurroundingChunkBlocks(ChunkID, Position);
			UpdateMesh(ChunkID);
		} else
		{
			if(Position.X >= CHUNK_WIDTH)
			{
				NoiseMap[FVector2D(ChunkID.X + 1, ChunkID.Y)].Set(0, Position.Y, Position.Z, 0);
				UpdateSurroundingChunkBlocks(ChunkID, Position);
				UpdateMesh(FVector2D(ChunkID.X + 1, ChunkID.Y));
				UpdateMesh(ChunkID);
			}
			if(Position.X < 0)
			{
				NoiseMap[FVector2D(ChunkID.X - 1, ChunkID.Y)].Set(CHUNK_WIDTH - 1, Position.Y, Position.Z, 0);
				UpdateSurroundingChunkBlocks(ChunkID, Position);
				UpdateMesh(FVector2D(ChunkID.X - 1, ChunkID.Y));
				UpdateMesh(ChunkID);
			}
			if(Position.Y >= CHUNK_WIDTH)
			{
				NoiseMap[FVector2D(ChunkID.X, ChunkID.Y + 1)].Set(Position.X, 0, Position.Z, 0);
				UpdateSurroundingChunkBlocks(ChunkID, Position);
				UpdateMesh(FVector2D(ChunkID.X, ChunkID.Y + 1));
				UpdateMesh(ChunkID);
			}
			if(Position.Y < 0)
			{
				NoiseMap[FVector2D(ChunkID.X, ChunkID.Y - 1)].Set(Position.X, CHUNK_WIDTH - 1, Position.Z, 0);
				UpdateSurroundingChunkBlocks(ChunkID, Position);
				UpdateMesh(FVector2D(ChunkID.X, ChunkID.Y - 1));
				UpdateMesh(ChunkID);
			}
		}
	}
}

void AChunkManager::PlaceBlock(const FVector WorldPosition, const FVector HitNormal)
{
	// Get the Chunk which you are placing a block in
	const FVector2D ChunkID = FVector2D(floor(WorldPosition.X / CHUNK_OFFSET), floor(WorldPosition.Y / CHUNK_OFFSET));

	// Get the position where to place the new block
	const FVector Position = FVector(floor(WorldPosition.X / BLOCK_SIZE + HitNormal.X / 2) - ChunkID.X * CHUNK_WIDTH,
										floor(WorldPosition.Y / BLOCK_SIZE + HitNormal.Y / 2) - ChunkID.Y * CHUNK_WIDTH, 
											floor(WorldPosition.Z / BLOCK_SIZE + HitNormal.Z / 2));
	
	if(NoiseMap.Find(ChunkID))
	{
		if(NoiseMap.operator[](ChunkID).Get(Position) >= 0)
		{
			NoiseMap[ChunkID].Set(Position.X, Position.Y, Position.Z, 1);
			UpdateSurroundingChunkBlocks(ChunkID, Position);
			UpdateMesh(ChunkID);
		} else
		{
			if(Position.X >= CHUNK_WIDTH)
			{
				NoiseMap[FVector2D(ChunkID.X + 1, ChunkID.Y)].Set(0, Position.Y, Position.Z, 1);
				UpdateMesh(FVector2D(ChunkID.X + 1, ChunkID.Y));
				UpdateMesh(ChunkID);
			}
			if(Position.X < 0)
			{
				NoiseMap[FVector2D(ChunkID.X - 1, ChunkID.Y)].Set(CHUNK_WIDTH - 1, Position.Y, Position.Z, 1);
				UpdateMesh(FVector2D(ChunkID.X - 1, ChunkID.Y));
				UpdateMesh(ChunkID);
			}
			if(Position.Y >= CHUNK_WIDTH)
			{
				NoiseMap[FVector2D(ChunkID.X, ChunkID.Y + 1)].Set(Position.X, 0, Position.Z, 1);
				UpdateMesh(FVector2D(ChunkID.X, ChunkID.Y + 1));
				UpdateMesh(ChunkID);
			}
			if(Position.Y < 0)
			{
				NoiseMap[FVector2D(ChunkID.X, ChunkID.Y - 1)].Set(Position.X, CHUNK_WIDTH - 1, Position.Z, 1);
				UpdateMesh(FVector2D(ChunkID.X, ChunkID.Y - 1));
				UpdateMesh(ChunkID);
			}
		}
	}
}

void AChunkManager::UpdateSurroundingChunkBlocks(const FVector2D ChunkID, const FVector Position)
{
	if(NoiseMap[ChunkID].Get(Position.X + 1, Position.Y, Position.Z) == -1) UpdateMesh(FVector2D(ChunkID.X + 1, ChunkID.Y));
	if(NoiseMap[ChunkID].Get(Position.X - 1, Position.Y, Position.Z) == -1) UpdateMesh(FVector2D(ChunkID.X - 1, ChunkID.Y));
	if(NoiseMap[ChunkID].Get(Position.X, Position.Y + 1, Position.Z) == -1) UpdateMesh(FVector2D(ChunkID.X, ChunkID.Y + 1));
	if(NoiseMap[ChunkID].Get(Position.X, Position.Y - 1, Position.Z) == -1) UpdateMesh(FVector2D(ChunkID.X, ChunkID.Y - 1));
}

void AChunkManager::SetBlockType(const int Sum)
{
	if(Sum >= 0) Cube.BlockVariations(BlockType::Sand);
	//if(Sum > 30) Cube->BlockVariations(BlockType::Dirt);
	//if(Sum > 50 + 30) Cube->BlockVariations(BlockType::Sand);
	//if(Sum > 58 + 30) Cube->BlockVariations(BlockType::Grass);
	//if(Sum > 76 + 30) Cube->BlockVariations(BlockType::Dirt);
	//if(Sum > 78 + 30) Cube->BlockVariations(BlockType::Stone);
	//if(Sum > 90 + 30) Cube->BlockVariations(BlockType::Dirt);
	//if(Sum > 93 + 30) Cube->BlockVariations(BlockType::Grass);
}




