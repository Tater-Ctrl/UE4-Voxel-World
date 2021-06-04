// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Cube = new FCubeCreation();

	// Colors for some reason?
	VertexColors.Init(FLinearColor(0.0f,0.0f,0.0f,0.5), 256);

	// Create new procedural mesh
	pm = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	SetRootComponent(pm);
	
	pm->CastShadow = true;
	pm->bUseAsyncCooking = true;
	
	IsActive = true;
}

int AChunk::CheckNeighbourChunk(int X, int Y, int Z, int IdX, int IdY)
{
	int Value = NoiseMap->operator[](FVector2D(IdX, IdY)).Get(X, Y, Z);
	
	return Value;
}

void AChunk::DrawChunk() 
{
	//pm->CreateMeshSection_LinearColor(0, Cube->Vertices, Cube->Triangles, Cube->Normals, Cube->Uvs, VertexColors, Tangents, true);
}

void AChunk::UpdateChunkMesh()
{
	pm->ClearMeshSection(0);
	CreateChunk();
}

void AChunk::CreateChunk()
{	
	Cube->ClearMeshData();

	double Start = FPlatformTime::Seconds();
	
	for(int X = 0; X < CHUNK_WIDTH; ++X)
	{
		for(int Y = 0; Y < CHUNK_WIDTH; ++Y)
		{
			for(int Z = 0; Z < CHUNK_HEIGHT; ++Z)
			{					
				if(NoiseMap->operator[](ChunkID).Get(X, Y, Z) == 0) continue;

				SetBlockType(NoiseMap->operator[](ChunkID).Get(X, Y, Z));
				
				// Draw top of block
				if(NoiseMap->operator[](ChunkID).Get(X, Y, Z + 1) == BlockType::Air)
				{
					Cube->Top(X, Y, Z );
				}

				// Draw bottom of block
				if(NoiseMap->operator[](ChunkID).Get(X, Y, Z - 1) == BlockType::Air)
				{
					Cube->Bottom(X, Y, Z);
				}
				
				// Draw front of block and check neighbour chunks
				if(NoiseMap->operator[](ChunkID).Get(X, Y + 1, Z) <= BlockType::Air)
				{
					// Check neighbour chunk if it has a block or not
					if(NoiseMap->operator[](ChunkID).Get(X, Y + 1, Z) < BlockType::Air && NoiseMap->Find(FVector2D(ChunkID.X, ChunkID.Y + 1)))
					{
						// if neighbour block is Air, display tile face
						if(NoiseMap->operator[](FVector2D(ChunkID.X, ChunkID.Y + 1)).Get(X, 0, Z) == BlockType::Air)
						{
							Cube->Front(X, Y, Z);
						}
					} else
					{
						Cube->Front(X, Y, Z);
					}
				}

				// Draw back of block  and check neighbour chunks
				if(NoiseMap->operator[](ChunkID).Get(X, Y - 1, Z) <= BlockType::Air)
				{
					// Check neighbour chunk if it has a block or not
					if(NoiseMap->operator[](ChunkID).Get(X, Y - 1, Z) < BlockType::Air && NoiseMap->Find(FVector2D(ChunkID.X, ChunkID.Y - 1)))
					{
						// if neighbour chunk block is Air, display tile face
						if(NoiseMap->operator[](FVector2D(ChunkID.X, ChunkID.Y - 1)).Get(X, CHUNK_WIDTH - 1, Z) == BlockType::Air)
							Cube->Back(X, Y, Z);
					} else
					{
						Cube->Back(X, Y, Z);
					}
				}
				
				// Draw left of block and check neighbour chunks
				if(NoiseMap->operator[](ChunkID).Get(X + 1, Y, Z) <= BlockType::Air)
				{
					// Check neighbour chunk if it has a block or not
					if(NoiseMap->operator[](ChunkID).Get(X + 1, Y, Z) < BlockType::Air && NoiseMap->Find(FVector2D(ChunkID.X + 1, ChunkID.Y)))
					{
						// if neighbour chunk block is Air, display tile face
						if(NoiseMap->operator[](FVector2D(ChunkID.X + 1, ChunkID.Y)).Get(0, Y, Z) == BlockType::Air)
							Cube->Left(X, Y, Z);
					} else
					{
						Cube->Left(X, Y, Z);
					}
				}
				
				// Draw right of block and check neighbour chunks
				if(NoiseMap->operator[](ChunkID).Get(X - 1, Y, Z) <= BlockType::Air)
				{
					// Check neighbour chunk if it has a block or not
					if(NoiseMap->operator[](ChunkID).Get(X - 1, Y, Z) < BlockType::Air && NoiseMap->Find(FVector2D(ChunkID.X - 1, ChunkID.Y)))
					{
						// if neighbour chunk block is Air, display tile face
						if(NoiseMap->operator[](FVector2D(ChunkID.X - 1, ChunkID.Y)).Get(CHUNK_WIDTH - 1, Y, Z) == BlockType::Air)
							Cube->Right(X, Y, Z);
					} else
					{
						// if neighbour chunk block is Air, display tile face
						Cube->Right(X, Y, Z);
					}
				}
			}
		}
	}
	
	this->DrawChunk();

	double End = FPlatformTime::Seconds();
	
	//UE_LOG(LogTemp, Warning, TEXT("%f ms"), (End-Start) * 1000);
}

void AChunk::SetBlockType(const int Sum)
{
	if(Sum >= 0) Cube->BlockVariations(BlockType::Sand);
	//if(Sum > 30) Cube->BlockVariations(BlockType::Dirt);
	//if(Sum > 50 + 30) Cube->BlockVariations(BlockType::Sand);
	//if(Sum > 58 + 30) Cube->BlockVariations(BlockType::Grass);
	//if(Sum > 76 + 30) Cube->BlockVariations(BlockType::Dirt);
	//if(Sum > 78 + 30) Cube->BlockVariations(BlockType::Stone);
	//if(Sum > 90 + 30) Cube->BlockVariations(BlockType::Dirt);
	//if(Sum > 93 + 30) Cube->BlockVariations(BlockType::Grass);
}




