// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeCreation.h"

// Sets default values
FCubeCreation::FCubeCreation()
{
	VerticesCount = 0;
}

void FCubeCreation::BlockVariations(const BlockType Type)
{
	switch(Type)
	{
		case BlockType::Grass:
			TextureTop = 0; TextureSide = 0; TextureBottom = 0;
		break;
		case BlockType::Dirt:
			TextureTop = 1; TextureSide = 1; TextureBottom = 1;
		break;
		case BlockType::Stone:
			TextureTop = 3; TextureSide = 3; TextureBottom = 3;
		break;
		case BlockType::Bedrock:
			TextureTop = 4; TextureSide = 4; TextureBottom = 4;
		break;
		case BlockType::Water:
			TextureTop = 5; TextureSide = 5; TextureBottom = 5;
		break;
		case BlockType::Sand:
			TextureTop = 6; TextureSide = 6; TextureBottom = 6;
		break;
		default:
			TextureTop = 1; TextureSide = 1; TextureBottom = 1;
		break;
	}
}

void FCubeCreation::AddNormals(const FVector Nor)
{
	Normals.Add(Nor);
	Normals.Add(Nor);
	Normals.Add(Nor);
	Normals.Add(Nor);
}

void FCubeCreation::AddTriangles()
{
	Triangles.Add(0 + Vertices.Num());
	Triangles.Add(1 + Vertices.Num());
	Triangles.Add(3 + Vertices.Num());
	Triangles.Add(1 + Vertices.Num());
	Triangles.Add(2 + Vertices.Num());
	Triangles.Add(3 + Vertices.Num());
}

void FCubeCreation::AddUV(const int Value)
{
	Uvs.Add(FVector2D(uvScale * (Uvx + Value), uvScale * Uvy));
	Uvs.Add(FVector2D(uvScale * (Uvx + Value), uvScale * Uvy - uvScale));
	Uvs.Add(FVector2D(uvScale * (Uvx + Value) - uvScale, uvScale * Uvy - uvScale));
	Uvs.Add(FVector2D(uvScale * (Uvx + Value) - uvScale, uvScale * Uvy));
}

void FCubeCreation::Top(const int X, const int Y, const int Z)
{	
	AddTriangles();
	AddUV(TextureTop);
	AddNormals(FVector(0.0f, 0.0f, 1.0f));

	Vertices.Add(FVector(X * BLOCK_SIZE, Y * BLOCK_SIZE, BLOCK_SIZE + Z * BLOCK_SIZE));
	Vertices.Add(FVector(X * BLOCK_SIZE, BLOCK_SIZE + Y * BLOCK_SIZE, BLOCK_SIZE + Z * BLOCK_SIZE));
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, BLOCK_SIZE + Y * BLOCK_SIZE, BLOCK_SIZE + Z * BLOCK_SIZE));
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, Y * BLOCK_SIZE, BLOCK_SIZE + Z * BLOCK_SIZE));

	VerticesCount += 4;
}

void FCubeCreation::Bottom(const int X, const int Y, const int Z)
{
	AddTriangles();
	AddUV(TextureBottom);
	AddNormals(FVector(0.0f, 0.0f, -1.0f));

	Vertices.Add(FVector(X * BLOCK_SIZE, BLOCK_SIZE + Y * BLOCK_SIZE, Z * BLOCK_SIZE));
	Vertices.Add(FVector(X * BLOCK_SIZE, Y * BLOCK_SIZE, Z * BLOCK_SIZE));
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, Y * BLOCK_SIZE, Z * BLOCK_SIZE));
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, BLOCK_SIZE + Y * BLOCK_SIZE, Z * BLOCK_SIZE));

	VerticesCount += 4;
}

void FCubeCreation::Front(const int X, const int Y, const int Z)
{
	AddTriangles();
	AddUV(TextureSide);
	AddNormals(FVector(0.0f, 1.0f, 0.0f));
	
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, BLOCK_SIZE + Y * BLOCK_SIZE, Z * BLOCK_SIZE));
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, BLOCK_SIZE + Y * BLOCK_SIZE, BLOCK_SIZE + Z * BLOCK_SIZE));
	Vertices.Add(FVector(X * BLOCK_SIZE, BLOCK_SIZE + Y * BLOCK_SIZE, BLOCK_SIZE + Z * BLOCK_SIZE));
	Vertices.Add(FVector(X * BLOCK_SIZE, BLOCK_SIZE + Y * BLOCK_SIZE, Z * BLOCK_SIZE));

	VerticesCount += 4;
}

void FCubeCreation::Back(const int X, const int Y, const int Z)
{
	AddTriangles();
	AddUV(TextureSide);
	AddNormals(FVector(0.0f, -1.0f, 0.0f));
	
	Vertices.Add(FVector(X * BLOCK_SIZE, Y * BLOCK_SIZE, Z * BLOCK_SIZE));
	Vertices.Add(FVector(X * BLOCK_SIZE, Y * BLOCK_SIZE, BLOCK_SIZE + Z * BLOCK_SIZE));
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, Y * BLOCK_SIZE, BLOCK_SIZE + (Z * BLOCK_SIZE)));
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, Y * BLOCK_SIZE, Z * BLOCK_SIZE));

	VerticesCount += 4;
}

void FCubeCreation::Left(const int X, const int Y, const int Z)
{
	AddTriangles();
	AddUV(TextureSide);
	AddNormals(FVector(1.0f, 0.0f, 0.0f));

	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, Y * BLOCK_SIZE, Z * BLOCK_SIZE));
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, Y * BLOCK_SIZE, BLOCK_SIZE + Z * BLOCK_SIZE));
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, BLOCK_SIZE + (Y * BLOCK_SIZE), BLOCK_SIZE + Z * BLOCK_SIZE));
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, BLOCK_SIZE + (Y * BLOCK_SIZE), Z * BLOCK_SIZE));

	VerticesCount += 4;
}

void FCubeCreation::Right(const int X, const int Y, const int Z)
{
	AddTriangles();
	AddUV(TextureSide);
	AddNormals(FVector(-1.0f, 0.0f, 0.0f));

	Vertices.Add(FVector(X * BLOCK_SIZE, BLOCK_SIZE + (Y * BLOCK_SIZE), Z * BLOCK_SIZE));
	Vertices.Add(FVector(X * BLOCK_SIZE, BLOCK_SIZE + (Y * BLOCK_SIZE), BLOCK_SIZE + Z * BLOCK_SIZE));
	Vertices.Add(FVector(X * BLOCK_SIZE, Y * BLOCK_SIZE, BLOCK_SIZE + (Z * BLOCK_SIZE)));
	Vertices.Add(FVector(X * BLOCK_SIZE, Y * BLOCK_SIZE, Z * BLOCK_SIZE));

	VerticesCount += 4;
}

void FCubeCreation::ClearMeshData()
{
	Vertices.Empty();
	Triangles.Empty();
	Uvs.Empty();
	Normals.Empty();
}