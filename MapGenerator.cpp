// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerator.h"

// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Simplex = new SimplexNoise();
	CaveGen = new CaveGenerator();
	BlockEditor = new ABlockEditor(&NoiseMap, &Chunks, &ChunkX, &ChunkY, &ChunkZ);
	
	TextureAtlas = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Texture_Atlas/Atlas_M.Atlas_M"));
}

void AMapGenerator::InitChunks()
{
	for(int X = -chunkLoaded / 2; X < chunkLoaded / 2; ++X)
	{
		for(int Y = -chunkLoaded / 2; Y < chunkLoaded / 2; ++Y)
		{
			if(!NoiseMap.Find(FVector2D(X + 1, Y))) CreateChunkNoise(FVector2D(X + 1, Y));
			if(!NoiseMap.Find(FVector2D(X - 1, Y))) CreateChunkNoise(FVector2D(X - 1, Y));
			if(!NoiseMap.Find(FVector2D(X, Y + 1))) CreateChunkNoise(FVector2D(X, Y + 1));
			if(!NoiseMap.Find(FVector2D(X, Y - 1))) CreateChunkNoise(FVector2D(X, Y - 1));

			if(!NoiseMap.Find(FVector2D(X, Y)))
			{
				CreateChunkNoise(FVector2D(X, Y));
			}

			EditorDrawChunk(FVector2D(X, Y));
		}
	}
}

void AMapGenerator::CreateChunk(const FVector2D Pos)
{
	AsyncTask(ENamedThreads::BackgroundThreadPriority, [=] ()
	{
		if(!NoiseMap.Find(FVector2D(Pos.X + 1, Pos.Y))) CreateChunkNoise(FVector2D(Pos.X + 1, Pos.Y));
		if(!NoiseMap.Find(FVector2D(Pos.X - 1, Pos.Y))) CreateChunkNoise(FVector2D(Pos.X - 1, Pos.Y));
		if(!NoiseMap.Find(FVector2D(Pos.X, Pos.Y + 1))) CreateChunkNoise(FVector2D(Pos.X, Pos.Y + 1));
		if(!NoiseMap.Find(FVector2D(Pos.X, Pos.Y - 1))) CreateChunkNoise(FVector2D(Pos.X, Pos.Y - 1));
		
		if(!NoiseMap.Find(Pos))
		{
			CreateChunkNoise(Pos);
		}

		AsyncTask(ENamedThreads::GameThread, [=]()
		{
			this->DrawChunk(Pos);
		});
	});
}

void AMapGenerator::CreateChunkNoise(FVector2D Pos)
{
	FMultiArray Arr(this->ChunkX, this->ChunkY, this->ChunkZ);
			
	for(int X = 0; X < this->ChunkX; ++X)
	{
		for(int Y = 0; Y < this->ChunkY; ++Y)
		{
			const int Height = ((this->Simplex->fractal2D(4, 0.002f, X + (Pos.X * this->ChunkX), Y + (Pos.Y * this->ChunkY))) + 1) * (this->ChunkZ / 2);
			const int Sum = floor(MapCurve->GetFloatValue(Height)) + 30;

			for(int Z = 0; Z < Sum; ++Z)
			{
				Arr.Set(X, Y, Z, Sum);
			}
		}
	}
	this->NoiseMap.Add(Pos, Arr);
}

void AMapGenerator::BreakBlock(const FVector Position, const FVector2D ChunkID)
{
	BlockEditor->BreakBlock(Position, ChunkID);
}

void AMapGenerator::PlaceBlock(const FVector Position, const FVector2D ChunkID)
{
	BlockEditor->PlaceBlock(Position, ChunkID);
}

void AMapGenerator::DestroyChunks()
{
	for( auto& e : Chunks)
	{
		e.Value->Destroy();
	}
	Chunks.Empty();
}

void AMapGenerator::EditorDrawChunk(const FVector2D Pos)
{
	AChunk* newChunk = GetWorld()->SpawnActor<AChunk>(FVector(Pos.X * 960, Pos.Y * 960, 0), FRotator(0.f, 0.f, 0.f));
	// Assign the X, Y, Z width pointers of chunk
	newChunk->chunkX = &this->ChunkX; newChunk->chunkY = &this->ChunkY;	newChunk->chunkZ = &this->ChunkZ;
	newChunk->chunkID = Pos;
	newChunk->pm->SetMaterial(0, this->TextureAtlas);
	newChunk->NoiseMap = &this->NoiseMap;

	newChunk->CreateChunk();
	Chunks.Add(Pos, newChunk);
}


void AMapGenerator::DrawChunk(const FVector2D Pos)
{
	AChunk* newChunk = GetWorld()->SpawnActor<AChunk>(FVector(Pos.X * 960, Pos.Y * 960, 0), FRotator(0.f, 0.f, 0.f));
	// Assign the X, Y, Z width pointers of chunk
	newChunk->chunkX = &this->ChunkX; newChunk->chunkY = &this->ChunkY;	newChunk->chunkZ = &this->ChunkZ;
	newChunk->chunkID = Pos;
	newChunk->pm->SetMaterial(0, this->TextureAtlas);
	newChunk->NoiseMap = &this->NoiseMap;

	CreateChunkQueue.Enqueue(newChunk);
}

void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if(Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success!"));
	}

	InitChunks();
}

void AMapGenerator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//If Player is active then update position around it
	if(Player)
	{
		const FVector2D Position = FVector2D(floor(Player->GetActorLocation().X / 960), floor(Player->GetActorLocation().Y / 960));
	
		if(Position != ChunkPosition)
		{
			ChunkPosition = Position;
			UpdateChunks(Position);
		}
	}
	if(!HideChunkQueue.IsEmpty())
	{
		// Remove 5 items from queue per tick or until empty
		for(int i = 0; i < 5 && !HideChunkQueue.IsEmpty(); ++i)
		{
			const int X = Player->GetActorLocation().X + (chunkLoaded / 2);
			const int Y = Player->GetActorLocation().Y + (chunkLoaded / 2);
			const int NX = Player->GetActorLocation().X - (chunkLoaded / 2);
			const int NY = Player->GetActorLocation().Y - (chunkLoaded / 2);

			AChunk* Tmp = nullptr;
			HideChunkQueue.Dequeue(Tmp);
	
			if(Tmp->chunkID.X > X || Tmp->chunkID.X < NX || Tmp->chunkID.Y > Y || Tmp->chunkID.Y < NY)
			{
				Tmp->SetActorHiddenInGame(true);
				Tmp->SetActorEnableCollision(false);
			} else
			{
				Tmp->IsActive = true;
			}
		}
	}

	if(!CreateChunkQueue.IsEmpty())
	{
		AChunk* Tmp = nullptr;
		CreateChunkQueue.Dequeue(Tmp);
		if(!Chunks.Find(Tmp->chunkID))
		{
			Tmp->CreateChunk();
			Chunks.Add(Tmp->chunkID, Tmp);
		}
	}
}

void AMapGenerator::UpdateChunks(FVector2D Pos)
{
	for(auto& e : Chunks)
	{
		if(e.Value->IsActive)
		{
			const int X = Pos.X + (chunkLoaded / 2);
			const int Y = Pos.Y + (chunkLoaded / 2);
			const int NX = Pos.X - (chunkLoaded / 2);
			const int NY = Pos.Y - (chunkLoaded / 2);
	
			if(e.Key.X > X || e.Key.X < NX || e.Key.Y > Y || e.Key.Y < NY)
			{
				HideChunkQueue.Enqueue(e.Value);
				e.Value->IsActive = false;
			}
		}
	}

	for(int x = -chunkLoaded / 2; x < chunkLoaded / 2; ++x)
	{
		for(int y = -chunkLoaded / 2; y < chunkLoaded / 2; ++y)
		{
			FVector2D Position = FVector2D(Pos.X + x, Pos.Y + y);

			// Unload old chunk if it has already been created
			if(Chunks.Find(Position))
			{
				Chunks[Position]->SetActorHiddenInGame(false);
				Chunks[Position]->SetActorEnableCollision(true);
				Chunks[Position]->IsActive = true;
			} else
			{
				// Create new chunk if it doesn't exist
				CreateChunk(Position);
			}		

		}
	}
}



