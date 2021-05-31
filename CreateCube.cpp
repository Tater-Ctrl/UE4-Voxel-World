// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateCube.h"

// Sets default values
ACreateCube::ACreateCube()
{
	VerticesCount = 0;
}

void ACreateCube::BlockVariations(const BlockType Type)
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

void ACreateCube::AddNormals(const FVector Nor)
{
	Normals.Add(Nor);
	Normals.Add(Nor);
	Normals.Add(Nor);
	Normals.Add(Nor);
}

void ACreateCube::AddTriangles()
{
	Triangles.Add(0 + Vertices.Num());
	Triangles.Add(1 + Vertices.Num());
	Triangles.Add(3 + Vertices.Num());
	Triangles.Add(1 + Vertices.Num());
	Triangles.Add(2 + Vertices.Num());
	Triangles.Add(3 + Vertices.Num());
}

void ACreateCube::AddUV(const int Value)
{
	Uvs.Add(FVector2D(uvScale * (Uvx + Value), uvScale * Uvy));
	Uvs.Add(FVector2D(uvScale * (Uvx + Value), uvScale * Uvy - uvScale));
	Uvs.Add(FVector2D(uvScale * (Uvx + Value) - uvScale, uvScale * Uvy - uvScale));
	Uvs.Add(FVector2D(uvScale * (Uvx + Value) - uvScale, uvScale * Uvy));
}

void ACreateCube::Top(const int X, const int Y, const int Z)
{	
	AddTriangles();
	AddUV(TextureTop);
	AddNormals(FVector(0.0f, 0.0f, 1.0f));

	Vertices.Add(FVector( X * BLOCK_SIZE, Y * BLOCK_SIZE, BLOCK_SIZE + Z * BLOCK_SIZE));
	Vertices.Add(FVector(X * BLOCK_SIZE, BLOCK_SIZE + Y * BLOCK_SIZE, BLOCK_SIZE + Z * BLOCK_SIZE));
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, BLOCK_SIZE + Y * BLOCK_SIZE, BLOCK_SIZE + Z * BLOCK_SIZE));
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, Y * BLOCK_SIZE, BLOCK_SIZE + Z * BLOCK_SIZE));

	VerticesCount += 4;
}

void ACreateCube::Bottom(const int X, const int Y, const int Z)
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

void ACreateCube::Front(const int X, const int Y, const int Z)
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

void ACreateCube::Back(const int X, const int Y, const int Z)
{
	AddTriangles();
	AddUV(TextureSide);
	AddNormals(FVector(0.0f, -1.0f, 0.0f));
	
	Vertices.Add(FVector(0 + X * BLOCK_SIZE, Y * BLOCK_SIZE, Z * BLOCK_SIZE));
	Vertices.Add(FVector(0 + X * BLOCK_SIZE, Y * BLOCK_SIZE, BLOCK_SIZE + Z * BLOCK_SIZE));
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, Y * BLOCK_SIZE, BLOCK_SIZE + (Z * BLOCK_SIZE)));
	Vertices.Add(FVector(BLOCK_SIZE + X * BLOCK_SIZE, Y * BLOCK_SIZE, Z * BLOCK_SIZE));

	VerticesCount += 4;
}

void ACreateCube::Left(const int X, const int Y, const int Z)
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

void ACreateCube::Right(const int X, const int Y, const int Z)
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

void ACreateCube::ClearMeshData()
{
	Vertices.Empty();
	Triangles.Empty();
	Uvs.Empty();
	Normals.Empty();
}