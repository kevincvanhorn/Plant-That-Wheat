#include "CGroundSection.h"
#include "ProceduralMeshComponent.h"
#include "CCharacterBase.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "CUsableActor.h"
#include "CMultiTool.h"
#include "PlantThatWheat.h"
#include "CVectorKDTree.h"

// Sets default values
ACGroundSection::ACGroundSection()
{
	PrimaryActorTick.bCanEverTick = true;

	ProcMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	ProcMeshComp->bUseAsyncCooking = true;
	RootComponent = ProcMeshComp;
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

/* Initialize and return a Ground Section Actor from a single face vertex set. */
ACGroundSection* ACGroundSection::CREATE(const UObject* WorldContextObject, FTransform SpawnTransform, TArray<FVector> Vertices) {
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);

	ACGroundSection* MyActor = World->SpawnActorDeferred<ACGroundSection>(ACGroundSection::StaticClass(), SpawnTransform);

	MyActor->PreSpawnInitialize(Vertices);

	UGameplayStatics::FinishSpawningActor(MyActor, SpawnTransform);

	return MyActor;
}

ACGroundSection* ACGroundSection::CREATE(const UObject* WorldContextObject, FTransform SpawnTransform, TArray<FVector> AllVertices, TArray<int32> VertsPerFace, UMaterial* GroundSectionMaterial) {
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	ACGroundSection* MyActor = World->SpawnActorDeferred<ACGroundSection>(ACGroundSection::StaticClass(), SpawnTransform);

	MyActor->PreSpawnInitialize(AllVertices, VertsPerFace);
	MyActor->GroundSectionMaterial = GroundSectionMaterial;
	//MyActor->Octree = new CVectorOctree(MyActor->GetActorLocation(), MyActor->GetActorScale3D()); 

	UGameplayStatics::FinishSpawningActor(MyActor, SpawnTransform);
	return MyActor;
}

/* Initialization before BeginPlay() after Actor Creation for creating sections for the entire HexGrid. */ 
void ACGroundSection::PreSpawnInitialize(TArray<FVector> AllVertices, TArray<int32> VertsPerFace) {
	bCreateSubsections = true;

	//TODO: Optimize - avoid copying over arrays.
	for (int32 i = 0; i < AllVertices.Num(); i++) {
		this->AllVertices.Emplace(AllVertices[i]);
	}
	for (int32 i = 0; i < VertsPerFace.Num(); i++) {
		this->VertsPerFace.Emplace(VertsPerFace[i]);
	}
}

/* Initialization before BeginPlay() after Actor Creation to set vertices. */
void ACGroundSection::PreSpawnInitialize(TArray<FVector> Vertices) {
	bCreateSubsections = false;
	for (int32 i = 0; i < Vertices.Num(); i++) {
		this->Vertices.Emplace(Vertices[i]);
	}
}

// Called when the game starts or when spawned
void ACGroundSection::BeginPlay()
{
	Super::BeginPlay();

	if (bCreateSubsections) {
		CreateAllSections();
	}
	else {
		AddSectionTriangles(Vertices.Num(), 0);
		CreateSectionFace(0);
	}
	//ProcMeshComp->ContainsPhysicsTriMeshData(true); // Enable collision data.
}

void ACGroundSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Adds triangles for a given face:
void ACGroundSection::AddSectionTriangles(int32 NumVerts, int32 sectionIndex) {
	FVector Centroid = FVector::ZeroVector;
	float X = 0, Y = 0, Z = 0;

	if (NumVerts == 0) { return; }

	// Calculate center vertex;
	for (int32 i = 0; i < NumVerts; i++) {
		X += Vertices[i].X;
		Y += Vertices[i].Y;
		Z += Vertices[i].Z;
	}

	Centroid = FVector(X/NumVerts, Y/NumVerts, Z/NumVerts);
	Vertices.Emplace(Centroid);

	SectionMap.Emplace(sectionIndex, Centroid); // Map the middle vertex to sectionIndex.
	
	//Octree->Insert(Centroid, sectionIndex);

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

void ACGroundSection::CreateSectionFace(int32 sectionIndex)
{
	TArray<FLinearColor> VertexColors;

	// Create Mesh: 
	VertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), Triangles.Num());
	ProcMeshComp->CreateMeshSection_LinearColor(sectionIndex, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), false);
	ProcMeshComp->SetMeshSectionVisible(sectionIndex, false);
}

void ACGroundSection::CreateAllSections() {
	int32 vOffset = 0; // Offset of the current face AllVertices array.
	int32 sectionIndex = 0;

	// Create each face:
	for (int32 f = 0; f < VertsPerFace.Num(); f++) {
		for (int32 v = 0; v < VertsPerFace[f]; v++) {
			if (vOffset < AllVertices.Num()) {
				Vertices.Emplace(AllVertices[vOffset]);
				vOffset++;
			}
		}
		// Add face:
		AddSectionTriangles(VertsPerFace[f], sectionIndex);
		CreateSectionFace(sectionIndex);
		ProcMeshComp->SetMaterial(sectionIndex, GroundSectionMaterial);
		//ProcMeshComp->GetProcMeshSection(sectionIndex)->bEnableCollision = false;

		// Reset for next face:
		sectionIndex++;
		Vertices.Empty();
		Triangles.Empty();
	}
	//ProcMeshComp->ContainsPhysicsTriMeshData(true);
	ProcMeshComp->SetRenderInMainPass(false);
	ProcMeshComp->SetRenderCustomDepth(true);
	//ProcMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//ProcMeshComp->SetCollisionResponseToChannel(COLLISION_PLANTINGTOOL, ECollisionResponse::ECR_Block);

	KDTree = new CVectorKDTree(SectionMap);
}

bool ACGroundSection::RevealSection(FVector HitLocation)
{
	//float MinSquaredDistance = TNumericLimits<float>::Max();
	//float curDist;

	//if (SectionMap.Num() > 0) {
		// TODO: Optimize - Replace with Nearest Neighbor search octree implementation

		/*for (auto& Elem : SectionMap)
		{
			curDist = FMath::Square(HitLocation.X - Elem.Value.X) + FMath::Square(HitLocation.Y - Elem.Value.Y) + FMath::Square(HitLocation.Z - Elem.Value.Z);
			if (MinSquaredDistance > curDist) {
				CurSectionIndex = Elem.Key;
				MinSquaredDistance = curDist;
			}
		}*/
		CurSectionIndex = KDTree->GetNearestNeighbor(HitLocation);

		ProcMeshComp->SetMeshSectionVisible(CurSectionIndex, true);
		//ProcMeshComp->GetProcMeshSection(MinIndex)->bEnableCollision = true;// SetCollisionResponseToChannel(COLLISION_PLANTINGTOOL, ECollisionResponse::ECR_Block);

		return true;
	//}
	//return false;
}

