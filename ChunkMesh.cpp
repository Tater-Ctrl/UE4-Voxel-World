// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkMesh.h"

// Sets default values
AChunkMesh::AChunkMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<URuntimeMeshComponentStatic>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

void AChunkMesh::SetMaterial(UMaterialInterface* Material) const
{
	Mesh->SetMaterial(0, Material);
}

void AChunkMesh::CreateMesh(const TArray<FVector>& Vertices, const TArray<int>& Triangles, const TArray<FVector>& Normals, const TArray<FVector2D>& Uvs) const
{
	Mesh->CreateSectionFromComponents(0, 0, 0, Vertices, Triangles, Normals, Uvs,
										VertexColors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, true);
}

void AChunkMesh::UpdateMesh(const TArray<FVector>& Vertices, const TArray<int>& Triangles, const TArray<FVector>& Normals, const TArray<FVector2D>& Uvs) const
{
	Mesh->UpdateSectionFromComponents(0, 0, Vertices, Triangles, Normals, Uvs, VertexColors, Tangents);
}

void AChunkMesh::HideMesh()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AChunkMesh::ShowMesh()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}