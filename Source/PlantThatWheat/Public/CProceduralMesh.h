

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CProceduralMesh.generated.h"

class UProceduralMeshComponent;

UCLASS()
class PLANTTHATWHEAT_API ACProceduralMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACProceduralMesh();

private:
	void PostActorCreated();
	
	void PostLoad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * MeshComp;

private:
	void CreateTriangle();

	int32 AddVertex(FVector Vertex);
	int32 GetEdgeMidpoint(int32 vIndex1, int32 vIndex2);

	TArray<FVector> Vertices; // Array of vertices in icosphere.
	int32 vIndex; // Index of current vertex being added to Vertices.

	TMap<int64, int32> MiddlePointMap; // Map the index of a midpoint vertex to its parent 2 vertices.
};
	