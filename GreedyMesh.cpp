// Fill out your copyright notice in the Description page of Project Settings.


#include "GreedyMesh.h"
#include "MapVariables.h"

AGreedyMesh::AGreedyMesh()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	SetRootComponent(Mesh);

	Mesh->CastShadow = false;
	Mesh->bUseAsyncCooking = true;
}

AGreedyMesh::~AGreedyMesh()
{
}

void AGreedyMesh::AddIndices(bool Reverse = false)
{
	if(!Reverse)
	{
		Indices.Add(0 + Vertices.Num());
		Indices.Add(1 + Vertices.Num());
		Indices.Add(3 + Vertices.Num());
		Indices.Add(1 + Vertices.Num());
		Indices.Add(2 + Vertices.Num());
		Indices.Add(3 + Vertices.Num());
	} else
	{	Indices.Add(3 + Vertices.Num());
		Indices.Add(2 + Vertices.Num());
		Indices.Add(1 + Vertices.Num());
		Indices.Add(3 + Vertices.Num());
		Indices.Add(1 + Vertices.Num());
		Indices.Add(0 + Vertices.Num());
	}
}

void AGreedyMesh::AddNormals(const FVector Nor)
{
	for (int i = 0; i < 4; ++i) {
		Normals.Add(Nor);
	}
}

void AGreedyMesh::CreateGreedyChunk(FVector2D ChunkID)
{
	Noise = &NoiseMap->operator[](ChunkID);
	double start = FPlatformTime::Seconds();
	for(int Z = 0; Z < CHUNK_HEIGHT; ++Z)
	{
		CreateTop(Z);
	}
	for(int X = 0; X < CHUNK_WIDTH; ++X)
	{
		CreateRight(X);
		CreateLeft(X);
	}
	for(int Y = 0; Y < CHUNK_WIDTH; ++Y)
	{
		CreateFront(Y);
		CreateBack(Y);
	}

	Mesh->CreateMeshSection_LinearColor(0, Vertices, Indices, Normals, Uvs, VertexColors, Tangents, true);

	double end = FPlatformTime::Seconds();

	//UE_LOG(LogTemp, Warning, TEXT("%f ms"), (end-start) * 1000);
}

void AGreedyMesh::CreateTop(const int Z)
{
	TArray<bool> CheckedBlock;

	CheckedBlock.Init(false, CHUNK_WIDTH * CHUNK_WIDTH);
	
	for(int X = 0; X < CHUNK_WIDTH; ++X)
	{
		for(int Y = 0; Y < CHUNK_WIDTH; ++Y)
		{
			if(!CheckedBlock[X + (Y * CHUNK_WIDTH)])
			{
				bool Greed = false;
				bool ExpandX = true;
				
				int AddX = X;
				int AddY = Y;

				int MaxLength = CHUNK_WIDTH;
			
				if(Noise->Get(X, Y, Z) > 0 && Noise->Get(X, Y, Z + 1) == 0)
				{
					AddIndices();
					AddNormals(FVector(0.0f, 0.0f, 1.0f));

					Vertices.Add(FVector(X * BLOCK_SIZE, Y * BLOCK_SIZE, (Z + 1) * BLOCK_SIZE));
				
					while(!Greed)
					{					
						if(Noise->Get(AddX, AddY, Z) > 0 && Noise->Get(AddX, AddY, Z + 1) == 0 && AddY < MaxLength && !CheckedBlock[AddX + (AddY * CHUNK_WIDTH)])
						{
							// Check neighbour if you can expand
							if(Noise->Get(AddX + 1, AddY, Z) <= 0 || Noise->Get(AddX + 1, AddY, Z + 1) != 0)
							{
								ExpandX = false;
							}
							
							CheckedBlock[AddX + (AddY * CHUNK_WIDTH)] = true;
							++AddY;
						} else
						{
							if(ExpandX)
							{
								if(AddX == X)
								{
									Vertices.Add(FVector(X * BLOCK_SIZE, AddY * BLOCK_SIZE, (Z + 1) * BLOCK_SIZE));
									MaxLength = AddY;
								}
								AddY = Y;
								++AddX;
							} else
							{
								if(AddX == X) Vertices.Add(FVector(X * BLOCK_SIZE, AddY * BLOCK_SIZE, (Z + 1) * BLOCK_SIZE));
								
								Vertices.Add(FVector((AddX + 1) * BLOCK_SIZE, AddY * BLOCK_SIZE, (Z + 1) * BLOCK_SIZE));
								Vertices.Add(FVector((AddX + 1) * BLOCK_SIZE, Y * BLOCK_SIZE, (Z + 1) * BLOCK_SIZE));
								Greed = true;
							}
						}
					}
				}
			}
		}
	}
}

void AGreedyMesh::CreateFront(const int Y)
{
	TArray<bool> CheckedBlock;

	CheckedBlock.Init(false, CHUNK_WIDTH * CHUNK_HEIGHT);
	
	for(int Z = 0; Z < CHUNK_HEIGHT; ++Z)
	{
		for(int X = 0; X < CHUNK_WIDTH; ++X)
		{
			if(!CheckedBlock[X + (Z * CHUNK_WIDTH)])
			{
				bool Greed = false;
				bool ExpandX = true;

				int AddX = X;
				int AddZ = Z;

				int MaxLength = CHUNK_WIDTH;
			
				if(Noise->Get(X, Y, Z) > 0 && Noise->Get(X, Y + 1, Z) == 0)
				{
					AddIndices();
					AddNormals(FVector(0.0f, 1.0f, 0.0f));
				
					while(!Greed)
					{				
						if(Noise->Get(AddX, Y, AddZ) > 0 && Noise->Get(AddX, Y + 1, AddZ) == 0 && AddX < MaxLength && !CheckedBlock[AddX + (AddZ * CHUNK_WIDTH)])
						{
							// Check neighbour if you can expand
							if(Noise->Get(AddX, Y, AddZ + 1) <= 0 || Noise->Get(AddX, Y + 1, AddZ + 1) != 0)
							{
								ExpandX = false;
							}
							
							CheckedBlock[AddX + (AddZ * CHUNK_WIDTH)] = true;
							++AddX;
						} else
						{
							if(ExpandX)
							{
								if(AddZ == Z)
								{
									Vertices.Add(FVector(AddX * BLOCK_SIZE, (Y + 1) * BLOCK_SIZE, Z * BLOCK_SIZE));
									MaxLength = AddX;
								}
								AddX = X;
								++AddZ;
							} else
							{
								if(AddZ == Z) Vertices.Add(FVector(AddX * BLOCK_SIZE, (Y + 1) * BLOCK_SIZE, Z * BLOCK_SIZE));
								
								Vertices.Add(FVector(AddX * BLOCK_SIZE, (Y + 1) * BLOCK_SIZE, (AddZ + 1) * BLOCK_SIZE));
								Vertices.Add(FVector(X * BLOCK_SIZE, (Y + 1) * BLOCK_SIZE, (AddZ + 1) * BLOCK_SIZE));
								Vertices.Add(FVector(X * BLOCK_SIZE, (Y + 1) * BLOCK_SIZE, Z * BLOCK_SIZE));
								Greed = true;
							}
						}
					}
				}
			}
		}
	}
}

void AGreedyMesh::CreateBack(const int Y)
{
	TArray<bool> CheckedBlock;

	CheckedBlock.Init(false, CHUNK_WIDTH * CHUNK_HEIGHT);
	
	for(int Z = 0; Z < CHUNK_HEIGHT; ++Z)
	{
		for(int X = 0; X < CHUNK_WIDTH; ++X)
		{
			if(!CheckedBlock[X + (Z * CHUNK_WIDTH)])
			{
				bool Greed = false;
				bool ExpandZ = true;

				int AddX = X;
				int AddZ = Z;

				int MaxLength = CHUNK_WIDTH;
			
				if(Noise->Get(X, Y, Z) > 0 && Noise->Get(X, Y - 1, Z) == 0)
				{
					AddIndices(true);
					AddNormals(FVector(0.0f, -1.0f, 0.0f));

					Vertices.Add(FVector(X * BLOCK_SIZE, Y * BLOCK_SIZE, Z * BLOCK_SIZE));
				
					while(!Greed)
					{	
						if(Noise->Get(AddX, Y, AddZ) > 0 && Noise->Get(AddX, Y - 1, AddZ) == 0 && AddX < MaxLength && !CheckedBlock[AddX + (AddZ * CHUNK_WIDTH)])
						{
							// Check neighbour if you can expand
							if(Noise->Get(AddX, Y, AddZ + 1) <= 0 || Noise->Get(AddX, Y - 1, AddZ + 1) != 0)
							{
								ExpandZ = false;
							}
							
							CheckedBlock[AddX + (AddZ * CHUNK_WIDTH)] = true;
							++AddX;
						} else
						{
							if(ExpandZ)
							{
								if(AddX == X)
								{
									Vertices.Add(FVector(AddX * BLOCK_SIZE, Y * BLOCK_SIZE, Z * BLOCK_SIZE));
									MaxLength = AddX;
								}
								AddX = X;
								++AddZ;
							} else
							{
								if(AddZ == Z) Vertices.Add(FVector(AddX * BLOCK_SIZE, Y * BLOCK_SIZE, Z * BLOCK_SIZE));
								
								Vertices.Add(FVector(AddX * BLOCK_SIZE, Y * BLOCK_SIZE, (AddZ + 1) * BLOCK_SIZE));
								Vertices.Add(FVector(X * BLOCK_SIZE, Y * BLOCK_SIZE, (AddZ + 1) * BLOCK_SIZE));
								Greed = true;
							}
						}
					}
				}
			}
		}
	}
}

void AGreedyMesh::CreateRight(const int X)
{
	TArray<bool> CheckedBlock;

	CheckedBlock.Init(false, CHUNK_WIDTH * CHUNK_HEIGHT);
	
	for(int Z = 0; Z < CHUNK_HEIGHT; ++Z)
	{
		for(int Y = 0; Y < CHUNK_WIDTH; ++Y)
		{
			if(!CheckedBlock[Y + (Z * CHUNK_WIDTH)])
			{
				bool Greed = false;
				bool ExpandX = true;

				int AddY = Y;
				int AddZ = Z;

				int MaxLength = CHUNK_WIDTH;
			
				if(Noise->Get(X, Y, Z) > 0 && Noise->Get(X - 1, Y, Z) == 0)
				{
					AddIndices();
					AddNormals(FVector(-1.0f, 0.0f, 0.0f));
				
					while(!Greed)
					{	
						if(Noise->Get(X, AddY, AddZ) > 0 && Noise->Get(X - 1, AddY, AddZ) == 0 && AddY < MaxLength && !CheckedBlock[AddY + (AddZ * CHUNK_WIDTH)])
						{
							// Check neighbour if you can expand
							if(Noise->Get(X, AddY, AddZ + 1) <= 0 || Noise->Get(X - 1, AddY, AddZ + 1) != 0)
							{
								ExpandX = false;
							}
							
							CheckedBlock[AddY + (AddZ * CHUNK_WIDTH)] = true;
							++AddY;
						} else
						{
							if(ExpandX)
							{
								if(AddZ == X)
								{
									Vertices.Add(FVector(X * BLOCK_SIZE, AddY * BLOCK_SIZE, Z * BLOCK_SIZE));
									MaxLength = AddY;
								}
								AddY = Y;
								++AddZ;
							} else
							{
								if(AddZ == Z) Vertices.Add(FVector(X * BLOCK_SIZE, AddY * BLOCK_SIZE, Z * BLOCK_SIZE));
								
								Vertices.Add(FVector(X * BLOCK_SIZE, AddY * BLOCK_SIZE, (AddZ + 1) * BLOCK_SIZE));
								Vertices.Add(FVector(X * BLOCK_SIZE, Y * BLOCK_SIZE, (AddZ + 1) * BLOCK_SIZE));
								Vertices.Add(FVector(X * BLOCK_SIZE, Y * BLOCK_SIZE, Z * BLOCK_SIZE));
								Greed = true;
							}
						}
					}
				}
			}
		}
	}
}

void AGreedyMesh::CreateLeft(const int X)
{
	TArray<bool> CheckedBlock;

	CheckedBlock.Init(false, CHUNK_WIDTH * CHUNK_HEIGHT);
	
	for(int Z = 0; Z < CHUNK_HEIGHT; ++Z)
	{
		for(int Y = 0; Y < CHUNK_WIDTH; ++Y)
		{
			if(!CheckedBlock[Y + (Z * CHUNK_WIDTH)])
			{
				bool Greed = false;
				bool ExpandX = true;

				int AddY = Y;
				int AddZ = Z;

				int MaxLength = CHUNK_WIDTH;
			
				if(Noise->Get(X, Y, Z) > 0 && Noise->Get(X + 1, Y, Z) == 0)
				{
					AddIndices(true);
					AddNormals(FVector(1.0f, 0.0f, 0.0f));

					Vertices.Add(FVector((X + 1) * BLOCK_SIZE, Y * BLOCK_SIZE, Z * BLOCK_SIZE));
				
					while(!Greed)
					{	
						if(Noise->Get(X, AddY, AddZ) > 0 && Noise->Get(X + 1, AddY, AddZ) == 0 && AddY < MaxLength && !CheckedBlock[AddY + (AddZ * CHUNK_WIDTH)])
						{
							// Check neighbour if you can expand
							if(Noise->Get(X, AddY, AddZ + 1) <= 0 || Noise->Get(X + 1, AddY, AddZ + 1) != 0)
							{
								ExpandX = false;
							}
							
							CheckedBlock[AddY + (AddZ * CHUNK_WIDTH)] = true;
							++AddY;
						} else
						{
							if(ExpandX)
							{
								if(AddZ == X)
								{
									Vertices.Add(FVector((X + 1) * BLOCK_SIZE, AddY * BLOCK_SIZE, Z * BLOCK_SIZE));
									MaxLength = AddY;
								}
								AddY = Y;
								++AddZ;
							} else
							{
								if(AddZ == Z) Vertices.Add(FVector((X + 1) * BLOCK_SIZE, AddY * BLOCK_SIZE, Z * BLOCK_SIZE));
								
								Vertices.Add(FVector((X + 1) * BLOCK_SIZE, AddY * BLOCK_SIZE, (AddZ + 1) * BLOCK_SIZE));
								Vertices.Add(FVector((X + 1) * BLOCK_SIZE, Y * BLOCK_SIZE, (AddZ + 1) * BLOCK_SIZE));
								Greed = true;
							}
						}
					}
				}
			}
		}
	}
}