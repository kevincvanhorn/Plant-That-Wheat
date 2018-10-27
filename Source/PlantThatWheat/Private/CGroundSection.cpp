#include "CGroundSection.h"
#include "ProceduralMeshComponent.h"
#include "CCharacterBase.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "CUsableActor.h"
#include "CMultiTool.h"

// Sets default values
ACGroundSection::ACGroundSection()
{
	bOutlineEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	MeshComp->bUseAsyncCooking = true;
	//MeshComp->SetCollisionEnabled(false);
	RootComponent = MeshComp;

	//Sample pentagon
	//Vertices.Emplace(FVector(-635.475, 200.078, 805.322));
	//Vertices.Emplace(FVector(-550.153, 252.810, 772.732));
	//Vertices.Emplace(FVector(-550.153, 252.810, 667.268));
	//Vertices.Emplace(FVector(-635.475, 200.078, 634.678));
	//Vertices.Emplace(FVector(-688.207, 167.488, 720.000));
}

bool ACGroundSection::OnUsed_Implementation(ACMultiTool * Tool)
{
	if (Tool->MyOwner) {
		if (Tool->MyOwner->ToolMode == EToolMode::Planting) {
			
			return true;
		}
	}
	return false;
}

bool ACGroundSection::StartFocus_Implementation()
{
	if (bOutlineEnabled) {
		MeshComp->SetRenderCustomDepth(true);
	}
	return true;
}

bool ACGroundSection::EndFocus_Implementation()
{
	if (bOutlineEnabled) {
		MeshComp->SetRenderCustomDepth(false);
	}
	return true;
}

// Called on Actor Spawn - ie. in the editor or at runtime.
void ACGroundSection::PostActorCreated() {
	Super::PostActorCreated();
	//CreateSectionFace();
}

// Called after map is opened & actor is in level.
void ACGroundSection::PostLoad() {
	Super::PostLoad();
	//CreateSectionFace();
}

/* Initialize and return a Ground Section Actor. */
ACGroundSection* ACGroundSection::CREATE(const UObject* WorldContextObject, FTransform SpawnTransform, TArray<FVector> Vertices) {
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);

	ACGroundSection* MyActor = World->SpawnActorDeferred<ACGroundSection>(ACGroundSection::StaticClass(), SpawnTransform);

	MyActor->PreSpawnInitialize(Vertices);

	UGameplayStatics::FinishSpawningActor(MyActor, SpawnTransform);

	return MyActor;
}

/* Initialization before BeginPlay() after Actor Creation to set vertices. */
void ACGroundSection::PreSpawnInitialize(TArray<FVector> Vertices) {
	for (int32 i = 0; i < Vertices.Num(); i++) {
		this->Vertices.Emplace(Vertices[i]);
	}
}

// Called when the game starts or when spawned
void ACGroundSection::BeginPlay()
{
	Super::BeginPlay();
	AddSectionTriangles();
	CreateSectionFace();
}

// Called every frame
void ACGroundSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Adds triangles for a given face:
void ACGroundSection::AddSectionTriangles() {
	FVector Centroid = FVector::ZeroVector;
	float X = 0, Y = 0, Z = 0;
	int32 NumVerts = Vertices.Num();

	if (NumVerts == 0) { return; }

	//UE_LOG(LogTemp, Warning, TEXT("%d"), NumVerts);

	//UE_LOG(LogTemp, Error, TEXT("%s"), *Vertices[0].ToString());

	// Calculate center vertex;
	for (int32 i = 0; i < NumVerts; i++) {
		X += Vertices[i].X;
		Y += Vertices[i].Y;
		Z += Vertices[i].Z;
	}

	//UE_LOG(LogTemp, Error, TEXT("%d"), X);
	//UE_LOG(LogTemp, Error, TEXT("%d"), Y);
	//UE_LOG(LogTemp, Error, TEXT("%d"), Z);

	Centroid = FVector(X/NumVerts, Y/NumVerts, Z/NumVerts);
	//float length = FMath::Sqrt(Centroid.X * Centroid.X + Centroid.Y * Centroid.Y + Centroid.Z * Centroid.Z);
	//Centroid = FVector(Centroid.X / length, Centroid.Y / length, Centroid.Z / length);

	UE_LOG(LogTemp, Error, TEXT("%s"), *Centroid.ToString());

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

void ACGroundSection::DisableOutlines()
{
	if (bOutlineEnabled) {
		MeshComp->SetRenderCustomDepth(false);
	}
}
