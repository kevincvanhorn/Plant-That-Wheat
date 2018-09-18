

#include "CProceduralMesh.h"
#include "ProceduralMeshComponent.h"


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
		for(VertexTriplet tri : TriangleVertices)
		{
			// replace triangle by 4 triangles
			int32 m1 = GetEdgeMidpoint(tri.Vert1, tri.Vert2);
			int32 m2 = GetEdgeMidpoint(tri.Vert2, tri.Vert3);
			int32 m3 = GetEdgeMidpoint(tri.Vert3, tri.Vert1);

			TrianglesSubdivided.Add(VertexTriplet{ tri.Vert1, m1, m3 });
			TrianglesSubdivided.Add(VertexTriplet{ tri.Vert2, m2, m1 });
			TrianglesSubdivided.Add(VertexTriplet{ tri.Vert3, m3, m2 });
			TrianglesSubdivided.Add(VertexTriplet{ m1, m2, m3 });
		}		

		TriangleVertices.Empty();// need to make tarray
		TriangleVertices.Append(TrianglesSubdivided);
	}

	// Set order of triangular faces by sequences of 3 vertices: 
	for (int32 i = 0; i < TriangleVertices.Num(); i++) {
		/*Triangles.Emplace(TriangleVertices[i].Vert1);
		Triangles.Emplace(TriangleVertices[i].Vert2);
		Triangles.Emplace(TriangleVertices[i].Vert3);*/
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
