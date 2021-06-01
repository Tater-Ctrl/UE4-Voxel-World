// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkManager.h"
#include "MapVariables.h"

// Sets default values
AChunkManager::AChunkManager()
{
	//Cube = new FCubeCreation();

	// Colors for some reason?
	VertexColors.Init(FLinearColor(0.0f,0.0f,0.0f,0.5), 256);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	RuntimeMeshComponent = CreateDefaultSubobject<URuntimeMeshComponentStatic>(TEXT("RuntimeMesh"));
	RuntimeMeshComponent->SetupAttachment(RootComp);

	RuntimeMeshComponent->CastShadow = false;

	TextureAtlas = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Texture_Atlas/Atlas_M.Atlas_M"));

	ChunkIndex = 0;
}

void AChunkManager::DrawChunk(FVector2D ChunkID) 
{
	ChunkSection.Emplace(ChunkID, ChunkIndex);

	RuntimeMeshComponent->SetMaterial(ChunkIndex, TextureAtlas);
	
	RuntimeMeshComponent->CreateSectionFromComponents(0, ChunkIndex, 0,
				Cube.Vertices, Cube.Triangles, Cube.Normals, Cube.Uvs, VertexColors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, true);
	++ChunkIndex;
}

void AChunkManager::UpdateChunkMesh()
{
	//pm->ClearMeshSection(0);
	//CreateChunk();
}

void AChunkManager::CreateChunk(const FVector2D ChunkID)
{	
	Cube.ClearMeshData();

	double Start = FPlatformTime::Seconds();

	int OffsetX = ChunkID.X * CHUNK_WIDTH;
	int OffsetY = ChunkID.Y * CHUNK_WIDTH;
	
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
					Cube.Top(X + OffsetX, Y + OffsetY, Z );
				}

				// Draw bottom of block
				if(NoiseMap[ChunkID].Get(X, Y, Z - 1) == BlockType::Air)
				{
					Cube.Bottom(X + OffsetX, Y + OffsetY, Z);
				}
				
				// Draw front of block and check neighbour chunks
				if(NoiseMap[ChunkID].Get(X, Y + 1, Z) == BlockType::Air)
				{
					Cube.Front(X + OffsetX, Y + OffsetY, Z);
					// Check neighbour chunk if it has a block or not
					// if(NoiseMap->operator[](ChunkID).Get(X, Y + 1, Z) < BlockType::Air && NoiseMap->Find(FVector2D(ChunkID.X, ChunkID.Y + 1)))
					// {
					// 	// if neighbour block is Air, display tile face
					// 	if(NoiseMap->operator[](FVector2D(ChunkID.X, ChunkID.Y + 1)).Get(X, 0, Z) == BlockType::Air)
					// 	{
					// 		Cube->Front(X, Y, Z);
					// 	}
					// } else
					// {
					// 	Cube->Front(X, Y, Z);
					// }
				}

				// Draw back of block  and check neighbour chunks
				if(NoiseMap[ChunkID].Get(X, Y - 1, Z) == BlockType::Air)
				{
					Cube.Back(X + OffsetX, Y + OffsetY, Z);
					// // Check neighbour chunk if it has a block or not
					// if(NoiseMap->operator[](ChunkID).Get(X, Y - 1, Z) < BlockType::Air && NoiseMap->Find(FVector2D(ChunkID.X, ChunkID.Y - 1)))
					// {
					// 	// if neighbour chunk block is Air, display tile face
					// 	if(NoiseMap->operator[](FVector2D(ChunkID.X, ChunkID.Y - 1)).Get(X, CHUNK_WIDTH - 1, Z) == BlockType::Air)
					// 		Cube->Back(X, Y, Z);
					// } else
					// {
					// 	Cube->Back(X, Y, Z);
					// }
				}
				
				// Draw left of block and check neighbour chunks
				if(NoiseMap[ChunkID].Get(X + 1, Y, Z) == BlockType::Air)
				{
					Cube.Left(X + OffsetX, Y + OffsetY, Z);
					// // Check neighbour chunk if it has a block or not
					// if(NoiseMap->operator[](ChunkID).Get(X + 1, Y, Z) < BlockType::Air && NoiseMap->Find(FVector2D(ChunkID.X + 1, ChunkID.Y)))
					// {
					// 	// if neighbour chunk block is Air, display tile face
					// 	if(NoiseMap->operator[](FVector2D(ChunkID.X + 1, ChunkID.Y)).Get(0, Y, Z) == BlockType::Air)
					// 		Cube->Left(X, Y, Z);
					// } else
					// {
					// 	Cube->Left(X, Y, Z);
					// }
				}
				
				// Draw right of block and check neighbour chunks
				if(NoiseMap[ChunkID].Get(X - 1, Y, Z) == BlockType::Air)
				{
					Cube.Right(X + OffsetX, Y + OffsetY, Z);
					// // Check neighbour chunk if it has a block or not
					// if(NoiseMap->operator[](ChunkID).Get(X - 1, Y, Z) < BlockType::Air && NoiseMap->Find(FVector2D(ChunkID.X - 1, ChunkID.Y)))
					// {
					// 	// if neighbour chunk block is Air, display tile face
					// 	if(NoiseMap->operator[](FVector2D(ChunkID.X - 1, ChunkID.Y)).Get(CHUNK_WIDTH - 1, Y, Z) == BlockType::Air)
					// 		Cube->Right(X, Y, Z);
					// } else
					// {
					// 	// if neighbour chunk block is Air, display tile face
					// 	Cube->Right(X, Y, Z);
					// }
				}
			}
		}
	}
	
	this->DrawChunk(ChunkID);

	double End = FPlatformTime::Seconds();
	
	//UE_LOG(LogTemp, Warning, TEXT("%f ms"), (End-Start) * 1000);
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




