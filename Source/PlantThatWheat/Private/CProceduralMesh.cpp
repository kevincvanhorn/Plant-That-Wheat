

#include "CProceduralMesh.h"
#include "ProceduralMeshComponent.h"
#include "DrawDebugHelpers.h"

/* http://www.flipcode.com/archives/The_Half-Edge_Data_Structure.shtml */

// Sets default values
ACProceduralMesh::ACProceduralMesh()
{
	MeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = MeshComp;
	MeshComp->bUseAsyncCooking = true;

	if (WITH_EDITOR)
	{
		PrimaryActorTick.bCanEverTick = true;
		PrimaryActorTick.bStartWithTickEnabled = true;
	}
}

bool ACProceduralMesh::ShouldTickIfViewportsOnly() const
{
	return true;
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
	SetDebugPoints();
}

void ACProceduralMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
	//UE_LOG(LogTemp, Warning, TEXT("Your message"));

	//FVector point = Vertices[Triangles[0]]*GetActorScale3D() + GetActorTransform().GetTranslation();
	//DrawDebugPoint(GetWorld(), point, 200, FColor(52, 220, 239), true);
	//DrawDebugSphere(GetWorld(), point, 50, 26, FColor(181, 0, 0), true, -1, 0, 2);

	// Draw Debug points
	/*for (int32 i = 0; i < DebugPoints.Num(); i++)
	{
		FVector point = DebugPoints[i] * GetActorScale3D() + GetActorTransform().GetTranslation();
		DrawDebugSphere(GetWorld(), point, 50, 26, FColor(181, 0, 0), true, 200, 0, 2);
	}*/
	/*if (!debugPointsSet) {
		debugPointsSet = true;
		SetDebugPoints();
	}*/

	/*for (int32 i = 0; i < DebugPoints.Num(); i++) {
		//DrawDebugSphere(GetWorld(), DebugPoints[i], 20, 26, FColor(255, 0, 0), true, -1, 0, 2); //red
		DrawDebugBox(GetWorld(), DebugPoints[i], FVector(25, 25, 25), FColor::Purple, true, -1, 0, 10);
		//DrawDebugString(GetWorld(), DebugPoints[i], FString::FromInt(i), nullptr, FColor::White, -1, true);
	}*/
	
}

void ACProceduralMesh::CreateTriangle() {
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

	int recursionLevel = 3;
	HE_edge *startEdge = nullptr;
	bool bStartEdgeSet = false;
	TArray<HE_edge*> StartingHalfEdges; // Tracks the location of the starting pentagon through subdivisions.

	//
	// refine triangles
	for (int i = 0; i < recursionLevel; i++)
	{
		TArray<VertexTriplet> TrianglesSubdivided;
		//TArray<HE_edge> HalfEdgesSubdivided;
		TMap<int64, HE_edge*> HalfEdgesSubdivided;

		for (VertexTriplet tri : TriangleVertices)
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

			// Forward Declare self-referential half-edges:
			HE_edge *Tri1_01 = nullptr, *Tri1_02 = nullptr, *Tri1_03 = nullptr, *Tri2_01 = nullptr, *Tri2_02 = nullptr, *Tri2_03 = nullptr,
				*Tri3_01 = nullptr, *Tri3_02 = nullptr, *Tri3_03 = nullptr, *TriM_01 = nullptr, *TriM_02 = nullptr, *TriM_03 = nullptr;

			// Subdivide Triangle into new half-edges:
			/*Tri1_01 = new HE_edge{ tri.Vert1, Tri1_02, nullptr };
			Tri1_02 = new HE_edge{ m1, Tri1_03, TriM_03 };
			Tri1_03 = new HE_edge{ m3, Tri1_01, nullptr };

			Tri2_01 = new HE_edge{ tri.Vert2, Tri2_02, nullptr };
			Tri2_02 = new HE_edge{ m2, Tri2_03, TriM_01 };
			Tri2_03 = new HE_edge{ m1, Tri2_01, nullptr };

			Tri3_01 = new HE_edge{ tri.Vert3, Tri3_02, nullptr };
			Tri3_02 = new HE_edge{ m3, Tri3_03, TriM_02 };
			Tri3_03 = new HE_edge{ m2, Tri3_01, nullptr };

			TriM_01 = new HE_edge{ m1,  TriM_02, Tri2_02 };
			TriM_02 = new HE_edge{ m2, TriM_03, Tri3_02 };
			TriM_03 = new HE_edge{ m3, TriM_01, Tri1_02 };*/

			// TEST: Create edge objects.
			Tri1_01 = new HE_edge{ tri.Vert1, nullptr, nullptr };
			Tri1_02 = new HE_edge{ m1, nullptr, nullptr };
			Tri1_03 = new HE_edge{ m3, nullptr, nullptr };

			Tri2_01 = new HE_edge{ tri.Vert2, nullptr, nullptr };
			Tri2_02 = new HE_edge{ m2, nullptr, nullptr };
			Tri2_03 = new HE_edge{ m1, nullptr, nullptr };

			Tri3_01 = new HE_edge{ tri.Vert3, nullptr, nullptr };
			Tri3_02 = new HE_edge{ m3, nullptr, nullptr };
			Tri3_03 = new HE_edge{ m2, nullptr, nullptr };

			TriM_01 = new HE_edge{ m1,  nullptr, nullptr };
			TriM_02 = new HE_edge{ m2, nullptr, nullptr };
			TriM_03 = new HE_edge{ m3, nullptr, nullptr };

			// TEST: Assign self referential pointers to edges.
			Tri1_01->next = Tri1_02;
			Tri1_02->next = Tri1_03; Tri1_02->pair = TriM_03; // Here, the pair edge is already defined with the triangle that was subdivided.
			Tri1_03->next = Tri1_01;

			Tri2_01->next = Tri2_02;
			Tri2_02->next = Tri2_03; Tri2_02->pair = TriM_01;
			Tri2_03->next = Tri2_01;

			Tri3_01->next = Tri3_02;
			Tri3_02->next = Tri3_03; Tri3_02->pair = TriM_02;
			Tri3_03->next = Tri3_01;

			TriM_01->next = TriM_02; TriM_01->pair = Tri2_02;
			TriM_02->next = TriM_03; TriM_02->pair = Tri3_02;
			TriM_03->next = TriM_01; TriM_03->pair = Tri1_02;

			//StartingHalfEdges.Emplace(Tri3_03);

			// Check Map & assign if opposite half edge pairs exist:
			if (HalfEdgesSubdivided.Contains(HE_edgeID{ tri.Vert1,m1 }.key()) && HalfEdgesSubdivided.Contains(HE_edgeID{ m1,tri.Vert2 }.key())) {
				Tri1_01->pair = HalfEdgesSubdivided[HE_edgeID{ tri.Vert1,m1 }.key()];
				Tri2_03->pair = HalfEdgesSubdivided[HE_edgeID{ m1,tri.Vert2 }.key()];
			}
			if (HalfEdgesSubdivided.Contains(HE_edgeID{ tri.Vert2, m2 }.key()) && HalfEdgesSubdivided.Contains(HE_edgeID{ m2,tri.Vert3 }.key())) {
				Tri2_01->pair = HalfEdgesSubdivided[HE_edgeID{ tri.Vert2, m2 }.key()];
				Tri3_03->pair = HalfEdgesSubdivided[HE_edgeID{ m2,tri.Vert3 }.key()];
			}
			if (HalfEdgesSubdivided.Contains(HE_edgeID{ tri.Vert3,m3 }.key()) && HalfEdgesSubdivided.Contains(HE_edgeID{ m3,tri.Vert1 }.key())) {
				Tri3_01->pair = HalfEdgesSubdivided[HE_edgeID{ tri.Vert3,m3 }.key()];
				Tri1_03->pair = HalfEdgesSubdivided[HE_edgeID{ m3, tri.Vert1 }.key()];
			}

			/*if ((*Tri1_02).next == nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("Tri1_02->next nullptr"));
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Tri1_02->next NICE!"));
			}*/

			/* Sets start edge with a vertex at the original 1st vertex of the icosahedron base.*/
			if (!bStartEdgeSet) {
				bStartEdgeSet = true;
				startEdge = Tri1_01; // halfedge points to m1 - This is the first halfedge of the icosahedron. 
			}

			//startEdge = TriM_01; // halfedge points to m1 - This is the first halfedge of the icosahedron. 


			// Add to Subdivided Map
			HalfEdgesSubdivided.Add(HE_edgeID{ m1,tri.Vert1 }.key(), Tri1_01);
			HalfEdgesSubdivided.Add(HE_edgeID{ m3,m1 }.key(), Tri1_02);
			HalfEdgesSubdivided.Add(HE_edgeID{ tri.Vert1, m3 }.key(), Tri1_03);

			HalfEdgesSubdivided.Add(HE_edgeID{ m2,tri.Vert2 }.key(), Tri2_01);
			HalfEdgesSubdivided.Add(HE_edgeID{ m1,m2 }.key(), Tri2_02);
			HalfEdgesSubdivided.Add(HE_edgeID{ tri.Vert2, m1 }.key(), Tri2_03);

			HalfEdgesSubdivided.Add(HE_edgeID{ m3,tri.Vert3 }.key(), Tri3_01);
			HalfEdgesSubdivided.Add(HE_edgeID{ m2,m3 }.key(), Tri3_02);
			HalfEdgesSubdivided.Add(HE_edgeID{ tri.Vert3, m2 }.key(), Tri3_03);

			HalfEdgesSubdivided.Add(HE_edgeID{ m2,m1 }.key(), TriM_01);
			HalfEdgesSubdivided.Add(HE_edgeID{ m3,m2 }.key(), TriM_02);
			HalfEdgesSubdivided.Add(HE_edgeID{ m1, m3 }.key(), TriM_03);
		}
		bStartEdgeSet = false; // reset startEdge once per loop

		// TriangleVertiecs = TrianglesSubdivided
		TriangleVertices.Empty();
		TriangleVertices.Append(TrianglesSubdivided);


		// HalfEdges = HalfEdgesSubdivided
		HalfEdgeMap.Empty();
		HalfEdgeMap.Append(HalfEdgesSubdivided);
	}

	/*if (StartingHalfEdges.Num() > 0) {
		startEdge = StartingHalfEdges[0];
	}*/

	/*
	for (int32 i = 0; HalfEdgeMap.Num(); i++) {
		DebugHalfEdges.Add(HalfEdgeMap)
	}*/

	// Set order of triangular faces by sequences of 3 vertices: 
	for (int32 i = 0; i < TriangleVertices.Num(); i++) {
		Triangles.Emplace(TriangleVertices[i].Vert3);
		Triangles.Emplace(TriangleVertices[i].Vert2);
		Triangles.Emplace(TriangleVertices[i].Vert1);

		DebugPoints.Emplace(Vertices[TriangleVertices[i].Vert3]);
		DebugPoints.Emplace(Vertices[TriangleVertices[i].Vert2]);
		DebugPoints.Emplace(Vertices[TriangleVertices[i].Vert1]);
	}



	/* Debug: temporarily set startedge vertex as 4th vertex drawn*/
	if (startEdge != nullptr) {
		int32 test = startEdge->vIndex;
		int32 test2 = startEdge->next->vIndex;
		int32 test3 = startEdge->next->next->vIndex;

		//int32 test3 = startEdge->next->next->vIndex;

		DebugPoints[0] = Vertices[test];
		DebugPoints[1] = Vertices[test2];
		DebugPoints[2] = Vertices[test3];


		//bugPoints.Emplace(Vertices[test]);
		//DebugPoints.Emplace(Vertices[test2]);
		//DebugPoints.Emplace(Vertices[test3]);
	}

	DebugPoints[3] = Vertices[(_TriangleVertices[0].Vert1)];
	DebugPoints[4] = Vertices[(_TriangleVertices[0].Vert2)];
	DebugPoints[5] = Vertices[( _TriangleVertices[0].Vert3)];

	VertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), Triangles.Num());

	MeshComp->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);

	// Enable collision data
	MeshComp->ContainsPhysicsTriMeshData(true);
}

void ACProceduralMesh::BuildHexagons() {
	// Start at first vertex pointing to edgeStart
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

/* Sets the scale of debug points by world transform and scale. */
void ACProceduralMesh::SetDebugPoints()
{
	for (int32 i = 0; i < DebugPoints.Num(); i++)
	{
		DebugPoints[i] = DebugPoints[i] * GetActorScale3D() + GetActorTransform().GetTranslation();
	}

	float size = 25;

	for (int32 i = 0; i < DebugPoints.Num() -3; i+=3) {
		//DrawDebugSphere(GetWorld(), DebugPoints[i], 20, 26, FColor(255, 0, 0), true, MAX_FLT, 0, 2); //red
		//DrawDebugBox(GetWorld(), DebugPoints[i], FVector(25, 25, 25), FColor::Red, true, MAX_FLT, 0, 10);
		//DrawDebugBox(GetWorld(), DebugPoints[i+1], FVector(25, 25, 25), FColor::Yellow, true, MAX_FLT, 0, 10);
		//DrawDebugBox(GetWorld(), DebugPoints[i+2], FVector(25, 25, 25), FColor::Black, true, MAX_FLT, 0, 10);

		//DrawDebugBox(GetWorld(), DebugPoints[i], FVector(size, size, size), FColor::Red, true, MAX_FLT, 0, 10);
		//DrawDebugBox(GetWorld(), DebugPoints[i+1], FVector(size, size, size), FColor::Yellow, true, MAX_FLT, 0, 10);
		//DrawDebugBox(GetWorld(), DebugPoints[i+2], FVector(size, size, size), FColor::Black, true, MAX_FLT, 0, 10);

		//DrawDebugPoint(GetWorld(), DebugPoints[i], 25, FColor(52, 220, 239), false, MAX_FLT);
		//DrawDebugPoint(GetWorld(), DebugPoints[i + 1], 25, FColor(52, 220, 239), false , MAX_FLT);
		//DrawDebugPoint(GetWorld(), DebugPoints[i + 2], 25, FColor(52, 220, 239), false, MAX_FLT);

		DrawDebugLine(GetWorld(), DebugPoints[i], DebugPoints[i+1], FColor::Emerald, true, MAX_FLT, 0, 10);
		DrawDebugLine(GetWorld(), DebugPoints[i+1], DebugPoints[i+2], FColor::Emerald, true, MAX_FLT, 0, 10);
		DrawDebugLine(GetWorld(), DebugPoints[i+2], DebugPoints[i], FColor::Emerald, true, MAX_FLT, 0, 10);
		size -= 1;
	}

	DrawDebugBox(GetWorld(), DebugPoints[0], FVector(20, 20, 20), FColor::Red, true, MAX_FLT, 0, 10);
	DrawDebugBox(GetWorld(), DebugPoints[1], FVector(20, 20, 20), FColor::Yellow, true, MAX_FLT, 0, 10);
	DrawDebugBox(GetWorld(), DebugPoints[2], FVector(20, 20, 20), FColor::Black, true, MAX_FLT, 0, 10);
	DrawDebugBox(GetWorld(), DebugPoints[3], FVector(15, 15, 15), FColor::Red, true, MAX_FLT, 0, 10);
	DrawDebugBox(GetWorld(), DebugPoints[4], FVector(15, 15, 15), FColor::Yellow, true, MAX_FLT, 0, 10);
	DrawDebugBox(GetWorld(), DebugPoints[5], FVector(15, 15, 15), FColor::Black, true, MAX_FLT, 0, 10);



	

	/*if (DebugPoints.Num() > 6) {
		DrawDebugSphere(GetWorld(), DebugPoints[0], 20, 26, FColor(255, 0, 0), true, MAX_FLT, 0, 2); //red
		DrawDebugSphere(GetWorld(), DebugPoints[1], 20, 26, FColor(255, 255, 255), true, MAX_FLT, 0, 2); // white
		DrawDebugSphere(GetWorld(), DebugPoints[2], 20, 26, FColor(0, 0, 0), true, MAX_FLT, 0, 2); // black
		DrawDebugSphere(GetWorld(), DebugPoints[3], 40, 26, FColor::Red, true, MAX_FLT, 0, 2); // green
		DrawDebugSphere(GetWorld(), DebugPoints[4], 40, 26, FColor::White, true, MAX_FLT, 0, 2); // green
		DrawDebugSphere(GetWorld(), DebugPoints[5], 40, 26, FColor::Black, true, MAX_FLT, 0, 2); // green

		//DrawDebugBox(GetWorld(), DebugPoints[0], FVector(25, 25, 25), FColor::Red, true, -1, 0, 10);
		//DrawDebugBox(GetWorld(), DebugPoints[1], FVector(25, 25, 25), FColor::Yellow, true, -1, 0, 10);
		//DrawDebugBox(GetWorld(), DebugPoints[2], FVector(25, 25, 25), FColor::Black, true, -1, 0, 10);
		//DrawDebugBox(GetWorld(), DebugPoints[3], FVector(25, 25, 25), FColor::Red, true, -1, 0, 10);
		//DrawDebugBox(GetWorld(), DebugPoints[4], FVector(25, 25, 25), FColor::Yellow, true, -1, 0, 10);
		//DrawDebugBox(GetWorld(), DebugPoints[5], FVector(25, 25, 25), FColor::Black, true, -1, 0, 10);

		DrawDebugLine(GetWorld(), DebugPoints[0], DebugPoints[1], FColor::Emerald, true, MAX_FLT, 0, 10);
		DrawDebugLine(GetWorld(), DebugPoints[1], DebugPoints[2], FColor::Emerald, true, MAX_FLT, 0, 10);
		DrawDebugLine(GetWorld(), DebugPoints[2], DebugPoints[0], FColor::Emerald, true, MAX_FLT, 0, 10);
	}*/
}
