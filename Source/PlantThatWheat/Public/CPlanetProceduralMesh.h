

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPlanetProceduralMesh.generated.h"

class UProceduralMeshComponent;

UCLASS()
class PLANTTHATWHEAT_API ACPlanetProceduralMesh : public AActor
{
	GENERATED_BODY()
	
public:
	ACPlanetProceduralMesh();

	/** The Procedural MeshComponent for the Planet - can be disabled. */
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent *MeshComp;

	/** Array of Hexagon (& pentagon) vertices on the calculated icosphere. */
	TArray<FVector> HexVertices;

	/** Array of face vert counts in HexVertices: Pentagon(5) vs. Hexagon(6). */
	TArray<int32> FaceSequence;

	/** For Deferred spawn initialization - to pass parameters before spawning actor. */
	void PreSpawnInitialize(FTransform _SpawnTransform, bool _bCreateProcMesh, bool _bDebugProcMesh);

	UFUNCTION(BlueprintCallable, Category = "Spawning", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true", BlueprintInternalUseOnly = "true"))
	static ACPlanetProceduralMesh* CREATE(const UObject* WorldContextObject, FTransform SpawnTransform, bool bCreateProcMesh, bool bDebugProcMesh);

protected:
	FTransform SpawnTransform;

	/** Half Edge - subset of the halfedge data structure. */
	struct HE_edge
	{
		int32 vIndex;           // Index of vertex at the end of the half-edge
		struct HE_edge* next;   // Next half-edge around the face - counter clockwise.
		struct HE_edge* pair;   // Oppositely oriented adjacent half-edge 
	};

	/** Unique identifier to choose out any Half Edge by its source and assigned vertex. ex: source ----> vertex */
	struct HE_edgeID
	{
		int32 source; // Index of source. ex: source ----> vertex (where -> is a triangle side)
		int32 vertex; // Index of vertex pointed to by half edge;
		int64 key() { return ((int64)source << 32) + (int64)vertex; }
	};

	/** Vector3D struct representation from vertiex indices. */
	struct VertexTriplet {
		int32 Vert1; int32 Vert2; int32 Vert3;
	};
	
	/** The number of triangle subdivisons applied to the mesh component from a base icosahedron. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Procedural Mesh")
	int32 RECURSION_LVL = 3;

	/** Should the debug points and halfedges of the constructed icosphere be displayed. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Procedural Mesh")
	bool bDebugProcMesh = false;

	/** Called on Actor Spawn - ie. in the editor or at runtime. */
	void PostActorCreated();

	/** Called after map is opened & actor is in level. */
	void PostLoad();

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Creates all of the needed properties for the Icosphere & Hexgrid. Returns true on completion */
	bool Initialize();

	/** Maps Half Edges to their vertex & source parent vertex (used in place of faces).*/
	TMap<int64, HE_edge*> HalfEdgeMap;

	/** Array of vertices in the icosphere. */
	TArray<FVector> Vertices;

	/** Maps the index of a midpoint vertex to its two parent vertices. */
	TMap<int64, int32> MiddlePointMap; 

/* General Construction variables: ------------------------------------------------------------------- */
private:
	/** Number of Edge Loops in the subdivided icosphere. */
	int32 NUM_RINGS;

	/** The middle edge loop of the subdivided icosphere. */
	int32 RING_MIDDLE;


/* Triangle Subdivision Stage: ----------------------------------------------------------------------- */
private:
	/** Initializes Vertices and Triangles from unit icosphere (Helper method of Initialize())*/
	void CreateStartingIcosphere();

	void CreateProcMesh(); // Create & Show the Procedural Mesh.
	
	/** Groups of triangles that make for the subdivided icosahedron. */
	TArray<VertexTriplet> TriangleVertices;

	/** Sequenced groups of 3 vertices for the triangles of the subdivided icosahedron. */
	TArray<int32> Triangles; // The triangles of the subdivided icosahedron.
	
	int32 vIndex; // Index of current vertex being added to Vertices.

	/** Only run through the initialization sequence of creating and subdividing triangles when false. */
	bool bHasBeenInitialized = false;

	/** Create Procedural mesh component when true.*/
	bool bShouldCreateProcMesh = false;

	/** Projects a vertex on the unit circle at the triangle subdivision stage. */
	int32 AddVertex(FVector Vertex);

	/** Checks if a midpoint exists, adding the vertex if not. */
	int32 GetEdgeMidpoint(int32 vIndex1, int32 vIndex2);


/* Hexagon Formation Stage: -------------------------------------------------------------------------- */
private:
	void BuildHexagons(HE_edge*	 edgeStart);

	void BuildFace(HE_edge* edgeStart);

	void BuildRing(HE_edge* edgeStart, int32 RingNum);

	void BuildRingOpp(HE_edge* edgeStart, int32 RingNum);

	/** Projects a vertex to the unit circle at the hexagon formation stage. */
	void AddHexVertex(FVector Vertex);

/* Debugging Information: --------------------------------------------------------------------------- */
private:
	TArray<FVector> DebugPoints;

	TArray<FVector> DebugHexPoints;

	TArray<HE_edge*> DebugHalfEdges;

	void SetDebugPoints();
};
