#include "CGroundSection.h"
#include "ProceduralMeshComponent.h"
#include "CCharacterBase.h"

// Sets default values
ACGroundSection::ACGroundSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = MeshComp;
	MeshComp->bUseAsyncCooking = true;
	
	//Sample pentagon
	Vertices.Emplace(FVector(-635.475, 200.078, 805.322));
	Vertices.Emplace(FVector(-550.153, 252.810, 772.732));
	Vertices.Emplace(FVector(-550.153, 252.810, 667.268));
	Vertices.Emplace(FVector(-635.475, 200.078, 634.678));
	Vertices.Emplace(FVector(-688.207, 167.488, 720.000));

	AddSectionTriangles();
}

// Called on Actor Spawn - ie. in the editor or at runtime.
void ACGroundSection::PostActorCreated() {
	Super::PostActorCreated();
	CreateSectionFace();
}

// Called after map is opened & actor is in level.
void ACGroundSection::PostLoad() {
	Super::PostLoad();
	CreateSectionFace();
}

// Called when the game starts or when spawned
void ACGroundSection::BeginPlay()
{
	Super::BeginPlay();
	
}

// _Implementation is auto-generated for BlueprintNativeEvents
bool ACGroundSection::OnInteract_Implementation(ACCharacterBase * Character)
{
	if (Character->ToolMode == EToolMode::Planting) {
		return true;
	}
	return false;
}

bool ACGroundSection::BeginFocus_Implementation()
{
	return false;
}

bool ACGroundSection::EndFocus_Implementation()
{
	return false;
}


// Called every frame
void ACGroundSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Adds triangles for a given face:
void ACGroundSection::AddSectionTriangles() {
	FVector Centroid = FVector::ZeroVector;
	int32 X = 0, Y = 0, Z = 0;
	int32 NumVerts = Vertices.Num();


	// Calculate center vertex;
	for (int32 i = 0; i < NumVerts; i++) {
		X += Vertices[i].X;
		Y += Vertices[i].Y;
		Z += Vertices[i].Z;
	}

	Centroid = FVector(X/NumVerts, Y/NumVerts, Z/NumVerts);
	Vertices.Emplace(Centroid);

	// Add Triangles counter-clockwise - Vertices are provided counterclockwise:
	// To make faces in opposite direction - do order i, i+1, m
	for (int32 i = 0; i < NumVerts-1; i++) {
		// Add all but last triangle:
		Triangles.Emplace(i);      
		Triangles.Emplace(NumVerts + 1); // NumVerts+1 is the newly added centroid vert.
		Triangles.Emplace(i + 1);        
	}

	// Add Last Triangle
	Triangles.Emplace(NumVerts - 1);     // Last Vertex
	Triangles.Emplace(NumVerts + 1);     // NumVerts+1 is the newly added centroid vert.
	Triangles.Emplace(0);                // First Vertex
}

void ACGroundSection::CreateSectionFace()
{
	TArray<FLinearColor> VertexColors;

	// Create Mesh: 
	VertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), Triangles.Num());
	MeshComp->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);

	// Enable collision data:
	MeshComp->ContainsPhysicsTriMeshData(true);

}
