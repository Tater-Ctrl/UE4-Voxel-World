// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockType.h"
#include "MapVariables.h"
#include "CubeCreation.generated.h"

USTRUCT()
struct PROCMAP_API FCubeCreation
{
	GENERATED_BODY()
private:
	float UvScale = 1.f / TEXTURE_ATLAS_SIZE;
	int Uvx = 1;
	int Uvy = 1;

	unsigned int TextureTop = 0;
	unsigned int TextureSide = 0;
	unsigned int TextureBottom = 0;

	void AddTriangles();
	void AddNormals(const FVector Nor);
	void AddUV(const int Value);

	unsigned int VerticesCount;

public:	
	// Sets default values for this actor's properties
	FCubeCreation();

	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<int> Triangles;
	TArray<FVector2D> Uvs;

	void Top(int X, int Y, int Z);
	void Bottom(int X, int Y, int Z);
	void Left(int X, int Y, int Z);
	void Right(int X, int Y, int Z);
	void Front(int X, int Y, int Z);
	void Back(int X, int Y, int Z);

	void BlockVariations(const BlockType Type);
	void ClearMeshData();
};
