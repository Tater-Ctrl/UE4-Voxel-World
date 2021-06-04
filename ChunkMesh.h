// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/RuntimeMeshComponentStatic.h"
#include "ChunkMesh.generated.h"

UCLASS()
class PROCMAP_API AChunkMesh : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AChunkMesh();

	bool IsActive;

	void SetMaterial(UMaterialInterface* Material) const;
	void CreateMesh(const TArray<FVector>& Vertices, const TArray<int>& Triangles, const TArray<FVector>& Normals, const TArray<FVector2D>& Uvs) const;
	void UpdateMesh(const TArray<FVector>& Vertices, const TArray<int>& Triangles, const TArray<FVector>& Normals, const TArray<FVector2D>& Uvs) const;
	void HideMesh();
	void ShowMesh();

private:
	UPROPERTY()
	URuntimeMeshComponentStatic* Mesh;

	TArray<FLinearColor> VertexColors;
	TArray<FRuntimeMeshTangent> Tangents;
};
