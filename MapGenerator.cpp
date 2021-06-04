#include "MapGenerator.h"
#include "ChunkManager.h"

// Sets default values

AMapGenerator::AMapGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TextureAtlas = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Texture_Atlas/Atlas_M.Atlas_M"));
}

void AMapGenerator::InitChunkRmc()
{ 
	if(MapCurve == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing MapCurve from MapGenerator! Returning"));
		return;
	}

	ChunkManager = GetWorld()->SpawnActor<AChunkManager>(FVector(0, 0, 0), FRotator(0 ,0 ,0)); 
	
	for(int X = -chunkLoaded / 2; X < chunkLoaded / 2; ++X)
	{
		for(int Y = -chunkLoaded / 2; Y < chunkLoaded / 2; ++Y)
		{
			if(!ChunkManager->NoiseMap.Find(FVector2D(X + 1, Y))) CreateChunkNoise(FVector2D(X + 1, Y));
			if(!ChunkManager->NoiseMap.Find(FVector2D(X - 1, Y))) CreateChunkNoise(FVector2D(X - 1, Y));
			if(!ChunkManager->NoiseMap.Find(FVector2D(X, Y + 1))) CreateChunkNoise(FVector2D(X, Y + 1));
			if(!ChunkManager->NoiseMap.Find(FVector2D(X, Y - 1))) CreateChunkNoise(FVector2D(X, Y - 1));
			
			if(!ChunkManager->NoiseMap.Find(FVector2D(X, Y)))
			{
				CreateChunkNoise(FVector2D(X, Y));
			}
			
			ChunkManager->DrawMesh(FVector2D(X, Y));
		}
	}
}

void AMapGenerator::CreateChunkNoise(const FVector2D Pos)
{
	FMultiArray Arr(CHUNK_WIDTH, CHUNK_WIDTH, CHUNK_HEIGHT);
			
	for(int X = 0; X < CHUNK_WIDTH; ++X)
	{
		for(int Y = 0; Y < CHUNK_WIDTH; ++Y)
		{
			const int Height = (Simplex.fractal2D(4, 0.004f, X + (Pos.X * CHUNK_WIDTH), Y + (Pos.Y * CHUNK_WIDTH)) + 1) * (CHUNK_HEIGHT / 2);
			const int Sum = floor(MapCurve->GetFloatValue(Height)) + 30;

			for(int Z = 0; Z < Sum; ++Z)
			{
				Arr.Set(X, Y, Z, 1);
			}
		}
	}
	this->ChunkManager->NoiseMap.Add(Pos, Arr);
}

void AMapGenerator::BreakBlock(const FVector Position, const FVector HitNormal)
{
	ChunkManager->BreakBlock(Position, HitNormal);
}

void AMapGenerator::PlaceBlock(const FVector Position, const FVector HitNormal)
{
	ChunkManager->PlaceBlock(Position, HitNormal);
}

void AMapGenerator::DestroyChunks()
{
	NoiseMap.Empty();
}

void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if(Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success!"));
	}

	InitChunkRmc();
}

void AMapGenerator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//If Player is active then update position around it
	 //if(Player)
	 //{
	 //	FVector2D Position = FVector2D(floor(Player->GetActorLocation().X / CHUNK_OFFSET), floor(Player->GetActorLocation().Y / CHUNK_OFFSET));
	 //
	 //	if(Position != ChunkPosition)
	 //	{
	 //		//UE_LOG(LogTemp, Warning, TEXT("%f, %f"), Position.X, Position.Y);
	 //		ChunkPosition = Position;
	 //		UpdateChunks(Position);
	 //	}
	 //}
	
	 //if(!HideChunkQueue.IsEmpty())
	 //{
	 //	// Remove 5 items from queue per tick or until empty
	 //	for(int i = 0; i < 5 && !HideChunkQueue.IsEmpty(); ++i)
	 //	{
	 //		const int X = Player->GetActorLocation().X + (chunkLoaded / 2);
	 //		const int Y = Player->GetActorLocation().Y + (chunkLoaded / 2);
	 //		const int NX = Player->GetActorLocation().X - (chunkLoaded / 2);
	 //		const int NY = Player->GetActorLocation().Y - (chunkLoaded / 2);
	 //
	 //		AChunk* Tmp = nullptr;
	 //		HideChunkQueue.Dequeue(Tmp);
	 //
	 //		if(Tmp->ChunkID.X > X || Tmp->ChunkID.X < NX || Tmp->ChunkID.Y > Y || Tmp->ChunkID.Y < NY)
	 //		{
	 //			Tmp->SetActorHiddenInGame(true);
	 //			Tmp->SetActorEnableCollision(false);
	 //		} else
	 //		{
	 //			Tmp->IsActive = true;
	 //		}
	 //	}
	 //}
	 
	 //if(!CreateChunkQueue.IsEmpty())
	 //{
	 //	AChunk* Tmp = nullptr;
	 //	CreateChunkQueue.Dequeue(Tmp);
	 //}
}

void AMapGenerator::UpdateChunks(FVector2D Pos)
{
	//for(auto& e : Chunks)
	//{
	//	if(e.Value->IsActive)
	//	{
	//		const int X = Pos.X + (chunkLoaded / 2);
	//		const int Y = Pos.Y + (chunkLoaded / 2);
	//		const int NX = Pos.X - (chunkLoaded / 2);
	//		const int NY = Pos.Y - (chunkLoaded / 2);
	//
	//		if(e.Key.X > X || e.Key.X < NX || e.Key.Y > Y || e.Key.Y < NY)
	//		{
	//			HideChunkQueue.Enqueue(e.Value);
	//			e.Value->IsActive = false;
	//		}
	//	}
	//}

	//for(int x = -chunkLoaded / 2; x < chunkLoaded / 2; ++x)
	//{
	//	for(int y = -chunkLoaded / 2; y < chunkLoaded / 2; ++y)
	//	{
	//		FVector2D Position = FVector2D(Pos.X + x, Pos.Y + y);
//
	//		// Unload old chunk if it has already been created
	//		if(Chunks.Find(Position))
	//		{
	//			Chunks[Position]->SetActorHiddenInGame(false);
	//			Chunks[Position]->SetActorEnableCollision(true);
	//			Chunks[Position]->IsActive = true;
	//		} else
	//		{
	//			// Create new chunk if it doesn't exist
	//			CreateChunk(Position);
	//		}		
	//	}
	//}
}
