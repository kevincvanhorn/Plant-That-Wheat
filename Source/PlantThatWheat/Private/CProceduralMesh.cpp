

#include "CProceduralMesh.h"
#include "ProceduralMeshComponent.h"

/* http://www.flipcode.com/archives/The_Half-Edge_Data_Structure.shtml */

// Sets default values
ACProceduralMesh::ACProceduralMesh()
{
	MeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = MeshComp;
	MeshComp->bUseAsyncCooking = true;
}

// Called on actor spawn - ie. in the editor or at runtime.
void ACProceduralMesh::PostActorCreated() {
	Super::PostActorCreated(); 
	CreateTriangle();
}

// Called after map is opened & actor is in level.
void ACProceduralMesh::PostLoad() { 
	Super::PostLoad(); 
	CreateTriangle();
}

// Called when the game starts or when spawned
void ACProceduralMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACProceduralMesh::CreateTriangle() {
	TArray<int32> Triangles;
	TArray<FLinearColor> VertexColors;
	vIndex = 0;

	//var s = Math.sqrt((5.0 - Math.sqrt(5.0)) / 10.0); // Put inital on unit sphere
	float G = (1.0 + FMath::Sqrt(5.0)) / 2.0; // Golden Ratio.
	const int32 NumVertices = 12;
	const int32 NumTriangles = 20;

	struct Coordinates {
		float X; float Y; float Z;
	};

	struct VertexTriplet {
		int32 Vert1; int32 Vert2; int32 Vert3;
	};

	Coordinates VertexCoordinates[NumVertices] = {
		{ -1, G, 0 }, { 1, G, 0 }, { -1, -G, 0 }, { 1, -G, 0 },
		{ 0, -1, G }, { 0, 1, G }, { 0, -1, -G }, { 0, 1, -G },
		{ G, 0, -1 }, { G, 0, 1 }, { -G, 0, -1 }, { -G, 0, 1 }
	};

	VertexTriplet _TriangleVertices[NumTriangles] =
	{
		{0,11,5}, {0,5,1}, {0,1,7}, {0,7,10}, {0,10,11},
		{1, 5, 9}, {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8}, 
		{3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9}, 
		{4, 9, 5}, {2, 4, 11}, {6, 2, 10}, {8, 6, 7}, {9, 8, 1}
	};

	TArray<VertexTriplet> TriangleVertices;

	// Establish Half-Edges
	//HE_edge HalfEdges[NumTriangles * 3] = {1, NULL, NULL};

	struct HE_edge_helper {
		int32 source;    // key to replace face identifier of half edges.
		int32 vertex;   // index of the halfEdgeVertext in Vertices[]
		int32 next;     // index of the next halfedge in HalfEdges[]
		int32 pair;    // index of the opposite halfedge in HalfEdges[]
	};

	HE_edge_helper HE[NumTriangles * 3] =
	{
		{11,0,1,14}, {5,11,2,18}, {0,5,0,3}, {5,0,4,2}, {1,5,5,15}, {0,1,3,6},
		{1,0,7,5}, {7.1,8,27}, {0,7,6,9}, {7,0,10,8}, {10,7,11,24}, {0,10,9,12},
		{10,0,13,11}, {11,10,14,21}, {0,11,12,0}, {5,1,16,4}, {9,5,17,46}, {1,9,15,59},
		{11,5,19,1}, {4,11,20,49}, {5,4,18,47}, {10,11,22,13}, {2,10,23,52}, {11,2,21,50},
		{7,10,25,10}, {6,7,26,55}, {10,6,24,53}, {1,7,28,7}, {8,1,29,58}, {7,8,27,56},
		{9,3,31,44}, {4,9,32,45}, {3,4,30,33}, {4,3,34,32}, {2,4,35,48}, {3,2,33,36},
		{2,3,37,35}, {6,2,38,51}, {3,6,36,39}, {6,3,40,38}, {8,6,41,54}, {3,8,39,42},
		{8,3,43,41}, {9,8,44,57}, {3,9,42,30}, {9,4,46,31}, {5,9,47,16}, {4,5,45,20},
		{4,2,49,34}, {11,4,50,19}, {2,11,48,23}, {2,6,52,37}, {10,2,53,22}, {6,10,51,26},
		{6,8,55,40}, {7,6,56,25}, {8,7,54,29}, {8,9,58,43}, {1,8,59,28}, {9,1,57,17}
	};

	/* Constuct Map of Half Edges.*/
	for (int i = 0; i < NumTriangles * 3; i++) {
		HalfEdgeMap.Add(HE_edgeID{HE[i].source,HE[i].vertex}.key(),
		                HE_edge{HE[i].vertex, &HalfEdges[HE[i].next], &HalfEdges[HE[i].pair] });
	}

	//TMap<int, HE_edge> HalfEdges;

	// Set order of triangular faces by sequences of 3 vertices: 
	for (int32 i = 0; i < NumTriangles; i++) {
		TriangleVertices.Emplace(_TriangleVertices[i]);
	}

	// Set locations of Vertices of Icosphere:
	for (int32 i = 0; i < NumVertices; i++) {
		AddVertex(FVector(
			VertexCoordinates[i].X,
			VertexCoordinates[i].Y,
			VertexCoordinates[i].Z));
	}

	int recursionLevel = 4;
	//
	// refine triangles
	for (int i = 0; i < recursionLevel; i++)
	{
		TArray<VertexTriplet> TrianglesSubdivided;
		TArray<HE_edge> HalfEdgesSubdivided;
		for(VertexTriplet tri : TriangleVertices)
		{
			// replace triangle by 4 triangles
			int32 m1 = GetEdgeMidpoint(tri.Vert1, tri.Vert2);
			int32 m2 = GetEdgeMidpoint(tri.Vert2, tri.Vert3);	
			int32 m3 = GetEdgeMidpoint(tri.Vert3, tri.Vert1);

			TrianglesSubdivided.Emplace(VertexTriplet{ tri.Vert1, m1, m3 });
			TrianglesSubdivided.Emplace(VertexTriplet{ tri.Vert2, m2, m1 });
			TrianglesSubdivided.Emplace(VertexTriplet{ tri.Vert3, m3, m2 });
			TrianglesSubdivided.Emplace(VertexTriplet{ m1, m2, m3 });

			/* Half Edges: */

			HE_edge *Tri1_01 = nullptr, *Tri1_02 = nullptr, *Tri1_03 = nullptr, *Tri2_01 = nullptr, *Tri2_02 = nullptr, *Tri2_03 = nullptr,
				*Tri3_01 = nullptr, *Tri3_02 = nullptr, *Tri3_03 = nullptr, *TriM_01 = nullptr, *TriM_02 = nullptr, *TriM_03 = nullptr;		

			Tri1_01 = new HE_edge{tri.Vert1, Tri1_02, nullptr}; 
			Tri1_02 = new HE_edge{m1, Tri1_03, TriM_03};//
			Tri1_03 = new HE_edge{m3, Tri1_01, nullptr};

			Tri2_01 = new HE_edge{tri.Vert2, Tri2_02, nullptr};
			Tri2_02 = new HE_edge{m2, Tri2_03, TriM_01};//
			Tri2_03 = new HE_edge{m1, Tri2_01, nullptr };

			Tri3_01 = new HE_edge{ tri.Vert3, Tri3_02, nullptr };
			Tri3_02 = new HE_edge{ m3, Tri3_03, TriM_02}; //
			Tri3_03 = new HE_edge{ m2, Tri3_01, nullptr};

			TriM_01 = new HE_edge{ m1,  TriM_02, Tri2_02}; //
			TriM_02 = new HE_edge{ m2, TriM_03, Tri3_02};   //
			TriM_03 = new HE_edge{ m3, TriM_01, Tri1_02}; //


			//TODO: Compare by checking ke and vertices[] for the x y coordinates bc duplicates are generated by the loop.
			/*if (HalfEdgeMap.Contains(HE_edgeID{ tri.Vert1,m1 }) && HalfEdgeMap.Contains(HE_edgeID{ m1,tri.Vert2 })) {

			}
			if (HalfEdgeMap.Contains(HE_edgeID{ tri.Vert2, m2 }) && HalfEdgeMap.Contains(HE_edgeID{ m2,tri.Vert3 })) {

			}
			if (HalfEdgeMap.Contains(HE_edgeID{ tri.Vert3,m3 }) && HalfEdgeMap.Contains(HE_edgeID{ m3,tri.Vert1 })) {

			}*/
		}		
		
		// TriangleVertiecs = TrianglesSubdivided
		TriangleVertices.Empty();
		TriangleVertices.Append(TrianglesSubdivided);

		// HalfEdges = HalfEdgesSubdivided
		HalfEdges.Empty();
		HalfEdges.Append(HalfEdgesSubdivided);
	}

	// Set order of triangular faces by sequences of 3 vertices: 
	for (int32 i = 0; i < TriangleVertices.Num(); i++) {
		Triangles.Emplace(TriangleVertices[i].Vert3);
		Triangles.Emplace(TriangleVertices[i].Vert2);
		Triangles.Emplace(TriangleVertices[i].Vert1);
	}

	VertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), Triangles.Num());

	MeshComp->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);

	// Enable collision data
	MeshComp->ContainsPhysicsTriMeshData(true);
}

// add vertex to mesh, fix position to be on unit sphere, return index
int32 ACProceduralMesh::AddVertex(FVector Vertex)
{
	float length = FMath::Sqrt(Vertex.X * Vertex.X + Vertex.Y * Vertex.Y + Vertex.Z * Vertex.Z);
	Vertices.Emplace(FVector(Vertex.X / length, Vertex.Y / length, Vertex.Z / length));
	return vIndex++; // Return then increment.

}

int32 ACProceduralMesh::GetEdgeMidpoint(int32 vIndex1, int32 vIndex2)
{
	//first check if we have it already
	int64 smallerindex = (vIndex1 < vIndex2) ? vIndex1 : vIndex2;
	int64 greaterindex = (vIndex1 < vIndex2) ? vIndex2 : vIndex1;
	int64 key = (smallerindex << 32) + greaterindex;

	if (MiddlePointMap.Contains(key))
	{
		return MiddlePointMap[key];
	}

	// not in cache, calculate it
	FVector v1 = Vertices[vIndex1];
	FVector v2 = Vertices[vIndex2];
	FVector vMiddle = FVector(
		(v1.X + v2.X) / 2,
		(v1.Y + v2.Y) / 2,
		(v1.Z + v2.Z) / 2);

	// add vertex makes sure point is on unit sphere
	int vMiddleIndex = AddVertex(vMiddle);

	// store it, return index
	MiddlePointMap.Add(key, vMiddleIndex);
	return vMiddleIndex;
}
