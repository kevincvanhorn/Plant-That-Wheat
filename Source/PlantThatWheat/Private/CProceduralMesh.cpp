

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

	HE_edge *startEdge = nullptr;
	bool bStartEdgeSet = false;
	TArray<HE_edge*> StartingHalfEdges; // Tracks the location of the starting pentagon through subdivisions.

	// Refine triangles:
	for (int i = 0; i < RECURSION_LVL; i++)
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

			// Check Map & assign if opposite half edge pairs exist: else create the keys with a null edge ptr.
			if (HalfEdgesSubdivided.Contains(HE_edgeID{ tri.Vert1,m1 }.key()) && HalfEdgesSubdivided.Contains(HE_edgeID{ m1,tri.Vert2 }.key())) {
				// If opposite half edge is in the map then just set this.pair.
				// Pairs for opposite half edges from v1 to v2:
				Tri1_01->pair = HalfEdgesSubdivided[HE_edgeID{ tri.Vert1,m1 }.key()];
				HalfEdgesSubdivided[HE_edgeID{ tri.Vert1,m1 }.key()]->pair = Tri1_01;
				
				Tri2_03->pair = HalfEdgesSubdivided[HE_edgeID{ m1,tri.Vert2 }.key()];
				HalfEdgesSubdivided[HE_edgeID{ m1,tri.Vert2 }.key()]->pair = Tri2_03;
			}
			
			else {
				// Create Half edge entry with pointer to this edge as pair and vertex defined.
				HalfEdgesSubdivided.Emplace(HE_edgeID{ tri.Vert1, m1 }.key(), new HE_edge{ m1, nullptr, Tri1_01 });
				HalfEdgesSubdivided.Emplace(HE_edgeID{ m1, tri.Vert2 }.key(), new HE_edge{ tri.Vert2, nullptr, Tri2_03 });
			}

			if (HalfEdgesSubdivided.Contains(HE_edgeID{ tri.Vert2, m2 }.key()) && HalfEdgesSubdivided.Contains(HE_edgeID{ m2,tri.Vert3 }.key())) {
				// Pairs for opposite half edges from v2 to v3:
				Tri2_01->pair = HalfEdgesSubdivided[HE_edgeID{ tri.Vert2, m2 }.key()];
				HalfEdgesSubdivided[HE_edgeID{ tri.Vert2, m2 }.key()]->pair = Tri2_01;

				Tri3_03->pair = HalfEdgesSubdivided[HE_edgeID{ m2,tri.Vert3 }.key()];
				HalfEdgesSubdivided[HE_edgeID{ m2,tri.Vert3 }.key()]->pair = Tri3_03;
			}
			else {
				HalfEdgesSubdivided.Emplace(HE_edgeID{ tri.Vert2, m2 }.key(), new HE_edge{m2, nullptr, Tri2_01});
				HalfEdgesSubdivided.Emplace(HE_edgeID{ m2, tri.Vert3 }.key(), new HE_edge{ tri.Vert3, nullptr, Tri3_03 });
			}

			if (HalfEdgesSubdivided.Contains(HE_edgeID{ tri.Vert3,m3 }.key()) && HalfEdgesSubdivided.Contains(HE_edgeID{ m3,tri.Vert1 }.key())) {
				Tri3_01->pair = HalfEdgesSubdivided[HE_edgeID{ tri.Vert3,m3 }.key()];
				HalfEdgesSubdivided[HE_edgeID{ tri.Vert3,m3 }.key()]->pair = Tri3_01;

				Tri1_03->pair = HalfEdgesSubdivided[HE_edgeID{ m3, tri.Vert1 }.key()];
				HalfEdgesSubdivided[HE_edgeID{ m3, tri.Vert1 }.key()]->pair = Tri1_03;

			}
			else {
				HalfEdgesSubdivided.Emplace(HE_edgeID{ tri.Vert3, m3 }.key(), new HE_edge{ m3, nullptr, Tri3_01 });
				HalfEdgesSubdivided.Emplace(HE_edgeID{ m3, tri.Vert1 }.key(), new HE_edge{ tri.Vert1, nullptr, Tri1_03 });
			}

			/* Sets start edge with a vertex at the original 1st vertex of the icosahedron base.*/
			if (!bStartEdgeSet) {
				bStartEdgeSet = true;
				startEdge = Tri1_01; // halfedge points to m1 - This is the first halfedge of the icosahedron. 
			}

			/* Add to Subdivided Map: */
			if (HalfEdgesSubdivided.Contains(HE_edgeID{ m1,tri.Vert1}.key()) && HalfEdgesSubdivided.Contains(HE_edgeID{ tri.Vert1, m3 }.key())) {
				// If a half edge exists then it has been previously defined with a vertex & pair, so it just needs a next.
				HalfEdgesSubdivided[HE_edgeID{ m1,tri.Vert1 }.key()]->next = Tri1_01->next;
				HalfEdgesSubdivided[HE_edgeID{ tri.Vert1, m3 }.key()]->next = Tri1_03->next;

			}
			else {
				HalfEdgesSubdivided.Add(HE_edgeID{ m1,tri.Vert1 }.key(), Tri1_01);
				HalfEdgesSubdivided.Add(HE_edgeID{ tri.Vert1, m3 }.key(), Tri1_03);
			}

			if (HalfEdgesSubdivided.Contains(HE_edgeID{ m2,tri.Vert2 }.key()) && HalfEdgesSubdivided.Contains(HE_edgeID{ tri.Vert2, m1 }.key())) {
				HalfEdgesSubdivided[HE_edgeID{ m2,tri.Vert2 }.key()]->next = Tri2_01->next;
				HalfEdgesSubdivided[HE_edgeID{ tri.Vert2, m1 }.key()]->next = Tri2_03->next;
			}
			else {
				HalfEdgesSubdivided.Add(HE_edgeID{ m2,tri.Vert2 }.key(), Tri2_01);
				HalfEdgesSubdivided.Add(HE_edgeID{ tri.Vert2, m1 }.key(), Tri2_03);
			}

			if (HalfEdgesSubdivided.Contains(HE_edgeID{ m3,tri.Vert3 }.key()) && HalfEdgesSubdivided.Contains(HE_edgeID{ tri.Vert3, m2 }.key())) {
				HalfEdgesSubdivided[HE_edgeID{ m3,tri.Vert3 }.key()]->next = Tri3_01->next;
				HalfEdgesSubdivided[HE_edgeID{ tri.Vert3, m2 }.key()]->next = Tri3_03->next;
			}
			else {
				HalfEdgesSubdivided.Add(HE_edgeID{ m3,tri.Vert3 }.key(), Tri3_01);
				HalfEdgesSubdivided.Add(HE_edgeID{ tri.Vert3, m2 }.key(), Tri3_03);
			}

			// Middle half edges are always new with subdivide.
			HalfEdgesSubdivided.Add(HE_edgeID{ m3,m1 }.key(), Tri1_02);
			HalfEdgesSubdivided.Add(HE_edgeID{ m1,m2 }.key(), Tri2_02);
			HalfEdgesSubdivided.Add(HE_edgeID{ m2,m3 }.key(), Tri3_02);

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

	// Set order of triangular faces by sequences of 3 vertices: 
	for (int32 i = 0; i < TriangleVertices.Num(); i++) {
		Triangles.Emplace(TriangleVertices[i].Vert3);
		Triangles.Emplace(TriangleVertices[i].Vert2);
		Triangles.Emplace(TriangleVertices[i].Vert1);

		DebugPoints.Emplace(Vertices[TriangleVertices[i].Vert3]);
		DebugPoints.Emplace(Vertices[TriangleVertices[i].Vert2]);
		DebugPoints.Emplace(Vertices[TriangleVertices[i].Vert1]);
	}

	/***************************************
	 * Debug Draw: BEGIN
	 *****************************************/

	/* Debug: temporarily set startedge vertex as 4th vertex drawn*/
	if (startEdge != nullptr) {
		int32 test = startEdge->vIndex;
		int32 test2 = startEdge->next->vIndex;
		int32 test3 = startEdge->next->next->vIndex;

		DebugPoints[0] = Vertices[test];
		DebugPoints[1] = Vertices[test2];
		DebugPoints[2] = Vertices[test3];
	}

	DebugPoints[3] = Vertices[(_TriangleVertices[0].Vert1)];
	DebugPoints[4] = Vertices[(_TriangleVertices[0].Vert2)];
	DebugPoints[5] = Vertices[( _TriangleVertices[0].Vert3)];

	/*****************************************
	 * Debug Draw: END 
	 *****************************************/

	VertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), Triangles.Num());

	MeshComp->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);

	// Enable collision data
	MeshComp->ContainsPhysicsTriMeshData(true);

	// Create Hexagons:
	BuildHexagons(startEdge);
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
	// First check if we have it already
	int64 smallerindex = (vIndex1 < vIndex2) ? vIndex1 : vIndex2;
	int64 greaterindex = (vIndex1 < vIndex2) ? vIndex2 : vIndex1;
	int64 key = (smallerindex << 32) + greaterindex;

	if (MiddlePointMap.Contains(key))
	{
		return MiddlePointMap[key];
	}

	// Not in cache, calculate it
	FVector v1 = Vertices[vIndex1];
	FVector v2 = Vertices[vIndex2];
	FVector vMiddle = FVector(
		(v1.X + v2.X) / 2,
		(v1.Y + v2.Y) / 2,
		(v1.Z + v2.Z) / 2);

	// Add vertex makes sure point is on unit sphere.
	int vMiddleIndex = AddVertex(vMiddle);

	// Store it, return index.
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
	for (int32 i = 0; i < DebugHexPoints.Num(); i++)
	{
			DebugHexPoints[i] = DebugHexPoints[i] * GetActorScale3D() + GetActorTransform().GetTranslation();
	}

	float size = 25;

	for (int32 i = 0; i < DebugPoints.Num(); i ++) {
			//DrawDebugBox(GetWorld(), DebugPoints[i], FVector(15, 15, 15), FColor::Emerald, true, MAX_FLT, 0, 10);
	}

	for (int32 i = 0; i < DebugHexPoints.Num(); i++) {
			DrawDebugBox(GetWorld(), DebugHexPoints[i], FVector(15, 15, 15), FColor::Cyan, true, MAX_FLT, 0, 10);
	}

	if (DebugHalfEdges.Num() >= 1) {
		FVector halfEdgeCur = Vertices[DebugHalfEdges[0]->vIndex];
		FVector halfEdgeNext = Vertices[DebugHalfEdges[0]->next->vIndex];
		FVector halfEdgeThird = Vertices[DebugHalfEdges[0]->next->next->vIndex];
		//FVector halfEdgeCur_2 = Vertices[DebugHalfEdges[1]->vIndex];
		//FVector halfEdgeNext_2 = Vertices[DebugHalfEdges[1]->next->vIndex];

		halfEdgeCur = halfEdgeCur * GetActorScale3D() + GetActorTransform().GetTranslation();
		halfEdgeNext = halfEdgeNext * GetActorScale3D() + GetActorTransform().GetTranslation();
		halfEdgeThird = halfEdgeThird * GetActorScale3D() + GetActorTransform().GetTranslation();
		//halfEdgeCur_2 = halfEdgeCur_2 * GetActorScale3D() + GetActorTransform().GetTranslation();
		//halfEdgeNext_2 = halfEdgeNext_2 * GetActorScale3D() + GetActorTransform().GetTranslation();

		DrawDebugBox(GetWorld(), halfEdgeCur, FVector(10, 10, 10), FColor::Red, true, MAX_FLT, 0, 10);
		DrawDebugBox(GetWorld(), halfEdgeNext, FVector(10, 10, 10), FColor::Yellow, true, MAX_FLT, 0, 10);
		DrawDebugBox(GetWorld(), halfEdgeThird, FVector(10, 10, 10), FColor::Black, true, MAX_FLT, 0, 10);
		//DrawDebugBox(GetWorld(), halfEdgeCur_2, FVector(10, 10, 10), FColor::Blue, true, MAX_FLT, 0, 10);
		//DrawDebugBox(GetWorld(), halfEdgeNext_2, FVector(10, 10, 10), FColor::Black, true, MAX_FLT, 0, 10);
	}
	
	for (int32 i = 0; i < DebugPoints.Num() -3; i+=3) {
		DrawDebugLine(GetWorld(), DebugPoints[i], DebugPoints[i+1], FColor::Emerald, true, MAX_FLT, 0, 10);
		DrawDebugLine(GetWorld(), DebugPoints[i+1], DebugPoints[i+2], FColor::Emerald, true, MAX_FLT, 0, 10);
		DrawDebugLine(GetWorld(), DebugPoints[i+2], DebugPoints[i], FColor::Emerald, true, MAX_FLT, 0, 10);
	}
	
	//DrawDebugBox(GetWorld(), DebugPoints[0], FVector(20, 20, 20), FColor::Red, true, MAX_FLT, 0, 10);
	//DrawDebugBox(GetWorld(), DebugPoints[1], FVector(20, 20, 20), FColor::Yellow, true, MAX_FLT, 0, 10);
	//DrawDebugBox(GetWorld(), DebugPoints[2], FVector(20, 20, 20), FColor::Black, true, MAX_FLT, 0, 10);
	//DrawDebugBox(GetWorld(), DebugPoints[3], FVector(15, 15, 15), FColor::Red, true, MAX_FLT, 0, 10);
	//DrawDebugBox(GetWorld(), DebugPoints[4], FVector(15, 15, 15), FColor::Yellow, true, MAX_FLT, 0, 10);
	//DrawDebugBox(GetWorld(), DebugPoints[5], FVector(15, 15, 15), FColor::Black, true, MAX_FLT, 0, 10);

}

void ACProceduralMesh::BuildHexagons(HE_edge* EdgeStart) {
	HE_edge* CurEdge = EdgeStart; 

	BuildFace(CurEdge); // Pentagon Center.
	CurEdge = CurEdge->pair;



	/**************************************************************
	 * Build Up to middle 3:
	 **************************************************************/
	
	for (int32 i = 0; i <= 3; i++) {
		BuildRing(CurEdge, i);
		//if (i != 1) {
			//CurEdge = CurEdge->pair->next->pair->next->pair->next->pair; // Go to next ring.
		CurEdge = CurEdge->next->next->pair->next->next->pair->next->next; // Same as above but goes to pentagon middle for expanding triangle at middle
		//}
	}

	/**************************************************************
	 * Build Middle 3:
	 **************************************************************/

	HE_edge* _TempEdge;
	int32 loopInt;

	//UE_LOG(LogTemp, Warning, FString::SanitizeFloat(NUM_RINGS));
	UE_LOG(LogTemp, Warning, TEXT("%d"), NUM_RINGS);

	for (int i = 0; i < 3; i++) {
		_TempEdge = CurEdge;
		loopInt = 0;
		do
		{
			BuildFace(CurEdge);
			CurEdge = CurEdge->next->pair->next->next->pair;
			loopInt++;
		} while (CurEdge != _TempEdge && loopInt < 100);

		CurEdge = CurEdge->next->next->pair->next->next->pair->next->next; // Same as above but goes to pentagon middle for expanding triangle at middle
	}

	/**************************************************************
	 * Build Down to Last vertex.
	 **************************************************************/

	loopInt = 0;
	do
	{
		BuildFace(CurEdge);
		CurEdge = CurEdge->next->pair->next->next->pair;
		loopInt++;
	} while (CurEdge != _TempEdge && loopInt < 100);

	
	// Reverse of build up to middle 3:

	CurEdge = CurEdge->pair->next->pair->next->pair->next->pair;

	for (int i = 3; i >= 0; i--) {
		BuildRingOpp(CurEdge, i);
		CurEdge = CurEdge->pair->next->pair->next->pair->next->pair;
	}


	DebugHalfEdges.Add(CurEdge);

	/******************************/
	for (int32 e = 0; e < HexVertices.Num(); e++) {
		//DebugPoints[6+e] = HexVertices[e];
		DebugHexPoints.Emplace(HexVertices[e]);
	}
	/*******************************/

	/*
	BuildRing(CurEdge, 0);
	CurEdge = CurEdge->pair->next->pair->next->pair->next->pair; // Go to next ring.
	BuildRing(CurEdge, 1); */

	//CurEdge = CurEdge->pair->next->pair->next->pair->next->pair;
	//CurEdge = CurEdge->next;// ->pair->next->pair->next;

	//DebugHalfEdges.Add(CurEdge);
	//BuildFace(EdgeStart); // First Hex Ring.

	//BuildRing(CurEdge, 1);
	
	// Next Ring
	//CurEdge = CurEdge->pair->next->pair->next->pair->next->pair;
	
	//DebugHalfEdges.Add(CurEdge);

	//BuildRing(CurEdge, 2);
	
	//BuildFace(CurEdge);
	//CurEdge = CurEdge->pair->next->next;
	//DebugHalfEdges.Add(CurEdge);
	//BuildFace(CurEdge);
	//////CurEdge = CurEdge->next->next->pair->next->pair;
	//CurEdge = CurEdge->pair->next->pair->next->next;
	//BuildFace(CurEdge);
	
	//BuildRing(CurEdge, 2);

	//CurEdge = CurEdge->next->pair->next->pair->next;

	// Start at first vertex pointing to EdgeStart
	//for (int32 i = 0; i < NUM_RINGS; i++) {
		 // first pentagon ring
		//CurEdge = CurEdge->pair->next->pair->next->pair->next->pair;
	//BuildRing(CurEdge);

	//}

	// Loop from first pentagon edge loop (ring) to last 
	//		Loop from vertex in ring back to first vertex
	//			Make face from all faces connected to the vertex
}



void ACProceduralMesh::BuildFace(HE_edge* EdgeStart) {
	HE_edge* CurEdge = EdgeStart;
	TArray<VertexTriplet> MidpointVertices;

	int32 debugCnt = 0;

	do
	{
		if (CurEdge->next != nullptr) {
			if (CurEdge->next->next != nullptr) {
				// Get all vertices of triangle:
				VertexTriplet TriVerts{ CurEdge->vIndex, CurEdge->next->vIndex, CurEdge->next->next->vIndex};
			
				// Get centroid & add to faces: 
				float X = (Vertices[TriVerts.Vert1].X + Vertices[TriVerts.Vert2].X + Vertices[TriVerts.Vert3].X) / 3;
				float Y = (Vertices[TriVerts.Vert1].Y + Vertices[TriVerts.Vert2].Y + Vertices[TriVerts.Vert3].Y) / 3;
				float Z = (Vertices[TriVerts.Vert1].Z + Vertices[TriVerts.Vert2].Z + Vertices[TriVerts.Vert3].Z) / 3;
				AddHexVertex(FVector(X, Y, Z));
			}
		}

		// Visit next triangle around vertex:
		if (CurEdge->next->next != nullptr) {
			if (CurEdge->next->next->pair != nullptr) {
				CurEdge = CurEdge->next->next->pair;
			}
		}
		debugCnt++;
	} while (CurEdge != EdgeStart && debugCnt < 6);//CurEdge != EdgeStart); // max of6 vertices for a face - ensure no infinite loop.
}

void ACProceduralMesh::BuildRing(HE_edge * EdgeStart, int32 RingNum)
{
	HE_edge* CurEdge = EdgeStart;
	int32 debugCnt = 0;

	int32 VertsPerEdge = RingNum + 1;//FMath::Exp2(RingNum);

	// Traverse from inside edges pointing to vertices of ring.
	// Visit each side: 
	do 
	{
		// Visit each vertex on a side:
		for (int i = 0; i < VertsPerEdge; i++) {
			BuildFace(CurEdge);
			if (i != VertsPerEdge - 1) {
				CurEdge = CurEdge->next->pair->next->next->pair;
			}
			else {
				CurEdge = CurEdge->next->pair;
			}
		}

		//CurEdge = CurEdge->next->next->pair->next->pair;
		// Even Edge, last vert of  edge is next-pair

		debugCnt += 1;
	} while (CurEdge != EdgeStart && debugCnt < 6);

}

void ACProceduralMesh::BuildRingOpp(HE_edge * EdgeStart, int32 RingNum)
{
	HE_edge* CurEdge = EdgeStart;
	int32 debugCnt = 0;

	int32 VertsPerEdge = RingNum + 1;//FMath::Exp2(RingNum);

	// Traverse from inside edges pointing to vertices of ring.
	// Visit each side: 
	do
	{
		// Visit each vertex on a side:
		for (int i = 0; i < VertsPerEdge; i++) {
			BuildFace(CurEdge);
			if (i != VertsPerEdge - 1) {
				CurEdge = CurEdge->next->next->pair->next->pair;
			}
			else {
				CurEdge = CurEdge->next->next->pair;
			}
		}

		//CurEdge = CurEdge->next->next->pair->next->pair;
		// Even Edge, last vert of  edge is next-pair

		debugCnt += 1;
	} while (CurEdge != EdgeStart && debugCnt < 6);

}

// add vertex to mesh, fix position to be on unit sphere
void ACProceduralMesh::AddHexVertex(FVector Vertex)
{
	float length = FMath::Sqrt(Vertex.X * Vertex.X + Vertex.Y * Vertex.Y + Vertex.Z * Vertex.Z);
	HexVertices.Emplace(FVector(Vertex.X / length, Vertex.Y / length, Vertex.Z / length));
}