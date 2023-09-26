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
		int32 vIndex;           // Index of vertex at the end of the half-edge
		struct HE_edge* next;   // Next half-edge around the face - counter clockwise.
		struct HE_edge* pair;   // Oppositely oriented adjacent half-edge 
	};

	// Unique identifier to choose out any Half Edge by its source and assigned vertex. ex: source ----> vertex
	struct HE_edgeID
	{
		int32 source;
		int32 vertex;
		int64 key() { return ((int64)source << 32) + (int64)vertex; }
	};

	/* Vector3D struct - for creating faces, tracking midpoints, etc.*/
	struct VertexTriplet {
		int32 Vert1; int32 Vert2; int32 Vert3;
	};

private:
	void PostActorCreated();

	void PostLoad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * MeshComp;

private:
	TMap<int64, HE_edge*> HalfEdgeMap; // Map Half Edges to their vertex & source parent vertex (used in place of faces).

	void CreateTriangle();

	int32 AddVertex(FVector Vertex);
	int32 GetEdgeMidpoint(int32 vIndex1, int32 vIndex2);

	TArray<FVector> Vertices; // Array of vertices in icosphere.
	int32 vIndex; // Index of current vertex being added to Vertices.

	TMap<int64, int32> MiddlePointMap; // Map the index of a midpoint vertex to its parent 2 vertices.

	TArray<int32> Triangles; // The triangles of the subdivided icosahedron.
	TArray<FVector> DebugPoints;
	TArray<FVector> DebugHexPoints;
	TArray<HE_edge*> DebugHalfEdges;

	void SetDebugPoints();
	bool debugPointsSet = false;

	// Create Hexagons Variables/Funcs:
	int32 RECURSION_LVL = 3; 
	int32 NUM_RINGS =  3;
	int32 RING_MIDDLE = NUM_RINGS / 2 + 1; // remove +1 if use ceiling function.

	void BuildHexagons(HE_edge*	 edgeStart);

	void BuildFace(HE_edge* edgeStart);
	void BuildRing(HE_edge* edgeStart, int32 RingNum);
	void BuildRingOpp(HE_edge* edgeStart, int32 RingNum);


	void AddHexVertex(FVector Vertex);

	TArray<FVector> HexVertices; // Array of vertices in icosphere.
};