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

	struct HE_edge
	{
		int32 vIndex;   // index of vertex at the end of the half-edge
		struct HE_edge* next;   // next half-edge around the face - counter clockwise.
		struct HE_edge* pair;   // oppositely oriented adjacent half-edge 
	};

	// Unique identifier to choose out any Half Edge by its source and assigned vertex. ex: source ----> vertex
	struct HE_edgeID
	{
		int32 source;
		int32 vertex;
		int64 key() { return ((int64)source << 32) + (int64)vertex; }
	};

	/*FORCEINLINE uint32 GetTypeHash(const HE_edgeID& b)
	{
		return FCrc::MemCrc_DEPRECATED(&b, sizeof(HE_edgeID));
	}*/

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTickIfViewportsOnly() const override;

private:
	void PostActorCreated();

	void PostLoad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent * MeshComp;

private:
	TArray<HE_edge> HalfEdges;

	TMap<int64, HE_edge*> HalfEdgeMap; // Map Half Edges to their vertex & source parent vertex (used in place of faces).

	void CreateTriangle();

	void BuildHexagons();

	int32 AddVertex(FVector Vertex);
	int32 GetEdgeMidpoint(int32 vIndex1, int32 vIndex2);

	//int64 Get64Mapping(HE_edgeID structIn);

	TArray<FVector> Vertices; // Array of vertices in icosphere.
	int32 vIndex; // Index of current vertex being added to Vertices.

	TMap<int64, int32> MiddlePointMap; // Map the index of a midpoint vertex to its parent 2 vertices.

	TArray<int32> Triangles; // The triangles of the subdivided icosahedron.
	TArray<FVector> DebugPoints;
	TArray<HE_edge> DebugHalfEdges;

	void SetDebugPoints();
	bool debugPointsSet = false;
};

