

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
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FLinearColor> VertexColors;

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

	VertexTriplet TriangleVertices[NumTriangles] =
	{
		{0,11,5}, {0,5,1}, {0,1,7}, {0,7,10}, {0,10,11},
		{1, 5, 9}, {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8}, 
		{3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9}, 
		{4, 9, 5}, {2, 4, 11}, {6, 2, 10}, {8, 6, 7}, {9, 8, 1}
	};

	// Set locations of Vertices of Icosphere:
	for (int32 i = 0; i < NumVertices; i++) {
		Vertices.Emplace(FVector(
			VertexCoordinates[i].X,
			VertexCoordinates[i].Y,
			VertexCoordinates[i].Z));
	}

	// Set order of triangular faces by sequences of 3 vertices: 
	for (int32 i = 0; i < NumTriangles; i++) {
		Triangles.Emplace(TriangleVertices[i].Vert1);
		Triangles.Emplace(TriangleVertices[i].Vert2);
		Triangles.Emplace(TriangleVertices[i].Vert3);
	}

	VertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), NumVertices);

	MeshComp->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);

	// Enable collision data
	MeshComp->ContainsPhysicsTriMeshData(true);
}

/*
//WORKING: Triangles
void ACProceduralMesh::CreateTriangle() {
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FLinearColor> VertexColors;

	const int32 NumVertices = 3;
	const int32 NumTriangles = 1;

	struct Coordinates {
		float X; float Y; float Z;
	};

	struct VertexTriplet {
		int32 Vert1; int32 Vert2; int32 Vert3;
	};

	Coordinates VertexCoordinates[NumVertices] = {
		{0.0f, 0.0f, 0.0f}, {100.0f,0.0f,0.0f}, {0.0f,100.0f,0.0f}
	};

	VertexTriplet TriangleVertices[NumTriangles] =
	{
		{0,2,1}
	};

	// Set locations of Vertices of Icosphere:
	for (int32 i = 0; i < NumVertices; i++) {
		Vertices.Emplace(FVector(
			VertexCoordinates[i].X,
			VertexCoordinates[i].Y,
			VertexCoordinates[i].Z));
	}

	// Set order of triangular faces by sequences of 3 vertices: 
	for (int32 i = 0; i < NumTriangles; i++) {
		Triangles.Emplace(TriangleVertices[i].Vert1);
		Triangles.Emplace(TriangleVertices[i].Vert2);
		Triangles.Emplace(TriangleVertices[i].Vert3);
	}

	VertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), NumVertices);

	MeshComp->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);

	// Enable collision data
	MeshComp->ContainsPhysicsTriMeshData(true);
}*/

/*void ACProceduralMesh::CreateTriangle() {
	TArray<FVector> vertices;
	TArray<int32> triangles;
	TArray<FLinearColor> VertexColors;
	vertices.Add(FVector(0.0f, 0.0f, 0.0f));
	vertices.Add(FVector(100.0f, 0.0f, 0.0f));
	vertices.Add(FVector(0.0f, 100.0f, 0.0f));

	triangles.Add(0);
	triangles.Add(2);
	triangles.Add(1);

	TArray<FVector2D> uvs;
	uvs.Init(FVector2D(0.0f, 0.0f), 3);
	TArray<FVector> normals;
	normals.Init(FVector(0.0f, 0.0f, 1.0f), 3);
	VertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), 3);
	TArray<FProcMeshTangent> tangents;
	tangents.Init(FProcMeshTangent(1.0f, 0.0f, 0.0f), 3);

	MeshComp->CreateMeshSection_LinearColor(0, vertices, triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);

	// Enable collision data
	MeshComp->ContainsPhysicsTriMeshData(true);
}*/

/*
void ACProceduralMesh::CreateTriangle() {
		const float X = .525731112119133606f;
		const float Z = .850650808352039932f;
		const float N = 0.f;

		TArray<FVector> Vertices;
		TArray<int32> Triangles;
		TArray<FLinearColor> VertexColors;

		struct Coordinates {
			float X; float Y; float Z;
		};

		struct VertexTriplet {
			int32 Vert1; int32 Vert2; int32 Vert3;
		};

		Coordinates VertexCoordinates[12] = {
			{-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z},
			{N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X},
			{Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}
		};

		VertexTriplet TriangleVerticies[20] = 
		{
			{0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
			{8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
			{7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
			{6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
		};

		/* Set locations of Vertices of Icosphere: */
		/*for (int32 i = 0; i < 12; i++) {
			Vertices.Emplace(FVector(
				VertexCoordinates[i].X,
				VertexCoordinates[i].Y,
				VertexCoordinates[i].Z));
		}*/

		//VertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), 12);
		
		/* Set order of triangular faces by sequences of 3 vertices: */
		/*for (int32 i = 0; i < 20; i++) {
			Triangles.Emplace(TriangleVerticies[i].Vert1);
			Triangles.Emplace(TriangleVerticies[i].Vert2);
			Triangles.Emplace(TriangleVerticies[i].Vert3);
		}
	
		MeshComp->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);

		// Enable collision data
		MeshComp->ContainsPhysicsTriMeshData(true);

		/*
		TArray<FVector> vertices;
		vertices.Add(FVector(0, 0, 0));
		vertices.Add(FVector(0, 100, 0)); 
		vertices.Add(FVector(0, 0, 100));

		TArray<FVector> normals;
		normals.Add(FVector(1, 0, 0));
		normals.Add(FVector(1, 0, 0));
		normals.Add(FVector(1, 0, 0));
		TArray<FVector2D> UV0;
		UV0.Add(FVector2D(0, 0));
		UV0.Add(FVector2D(10, 0));
		UV0.Add(FVector2D(0, 10));

		TArray<FProcMeshTangent> tangents; tangents.Add(FProcMeshTangent(0, 1, 0)); tangents.Add(FProcMeshTangent(0, 1, 0)); tangents.Add(FProcMeshTangent(0, 1, 0));
		TArray<FLinearColor> vertexColors; vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0)); vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0)); vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
		
		//MeshComp->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);

		// Enable collision data
		//MeshComp->ContainsPhysicsTriMeshData(true);
}*/
