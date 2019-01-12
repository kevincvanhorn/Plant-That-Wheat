#include "CGroundSection.h"
#include "ProceduralMeshComponent.h"
#include "CCharacterBase.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "CUsableActor.h"
#include "CMultiTool.h"
#include "PlantThatWheat.h"
#include "CVectorKDTree.h"
#include "DrawDebugHelpers.h"
#include "Engine/Classes/Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "CStaticFoliageComponent.h"
#include "CPlanetActor.h"


// Sets default values
ACGroundSection::ACGroundSection()
{
	PrimaryActorTick.bCanEverTick = true;

	ProcMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	ProcMeshComp->bUseAsyncCooking = true;
	RootComponent = ProcMeshComp;

	WheatComponent = CreateDefaultSubobject<UCStaticFoliageComponent>(TEXT("WheatComp"));
	if (WheatMesh) {
		WheatComponent->SetupAttachment(RootComponent);
	}
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
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);

	ACGroundSection* MyActor = World->SpawnActorDeferred<ACGroundSection>(ACGroundSection::StaticClass(), SpawnTransform);

	MyActor->PreSpawnInitialize(Vertices);

	UGameplayStatics::FinishSpawningActor(MyActor, SpawnTransform);

	return MyActor;
}

ACGroundSection* ACGroundSection::CREATE(const UObject* WorldContextObject, FTransform SpawnTransform, TArray<FVector> AllVertices, TArray<int32> VertsPerFace, UMaterial* GroundSectionMaterial, float HexGridScale, UStaticMesh* WheatMesh, ACPlanetActor* const Planet) {
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	ACGroundSection* MyActor = World->SpawnActorDeferred<ACGroundSection>(ACGroundSection::StaticClass(), SpawnTransform);

	MyActor->PreSpawnInitialize(AllVertices, VertsPerFace);
	MyActor->GroundSectionMaterial = GroundSectionMaterial;
	MyActor->HexGridScale = HexGridScale;
	MyActor->WheatMesh = WheatMesh;
	MyActor->PlanetOwner = Planet;
	//MyActor->Octree = new CVectorOctree(MyActor->GetActorLocation(), MyActor->GetActorScale3D()); 

	if (MyActor->WheatComponent && WheatMesh) {
		MyActor->WheatComponent->SetStaticMesh(WheatMesh);
		MyActor->WheatComponent->PlanetOwner = Planet;

		// Defaults as Collision Object:: Dynamic.
		MyActor->WheatComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		MyActor->WheatComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		MyActor->WheatComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	}

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
		AddSectionTriangles(Vertices.Num(), 0, 0);
		CreateSectionFace(0);
	}
	//ProcMeshComp->ContainsPhysicsTriMeshData(true); // Enable collision data.
}

void ACGroundSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACGroundSection::AddSectionTriangles(int32 NumVerts, int32 sectionIndex, int32 StartIndex) {
	FVector Centroid = FVector::ZeroVector;
	float X = 0, Y = 0, Z = 0;

	TArray<FVector*> ShapeVertices; // For Wheat

	if (NumVerts == 0) { return; }

	// Calculate center vertex;
	for (int32 i = 0; i < NumVerts; i++) {
		X += Vertices[i].X;
		Y += Vertices[i].Y;
		Z += Vertices[i].Z;

		if (StartIndex < AllVertices.Num()) {
			ShapeVertices.Emplace(new FVector(AllVertices[StartIndex + i] * HexGridScale));
		}
	}

	Centroid = FVector(X / NumVerts, Y / NumVerts, Z / NumVerts);

	Vertices.Emplace(Centroid);

	// Add Triangles counter-clockwise - Vertices are provided counterclockwise:
	// To make faces in opposite direction - do order i, i+1, m
	for (int32 i = 0; i < NumVerts - 1; i++) {
		// Add all but last triangle:
		Triangles.Emplace(i);
		Triangles.Emplace(NumVerts + 1); // NumVerts+1 is the newly added centroid vert.
		Triangles.Emplace(i + 1);
	}

	// Add Last Triangle
	Triangles.Emplace(NumVerts - 1);     // Last Vertex
	Triangles.Emplace(NumVerts + 1);     // NumVerts+1 is the newly added centroid vert.
	Triangles.Emplace(0);                // First Vertex

	SectionMap.Emplace(sectionIndex, WheatInfo{ new FVector(Centroid*HexGridScale), false, ShapeVertices, ShapeVertices, 
												CalculateNormal(Centroid*HexGridScale), 0, false});
	CalculateDistributedVerts(sectionIndex);
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
	int32 StartIndex = 0;

	// Create each face:
	for (int32 f = 0; f < VertsPerFace.Num(); f++) {
		for (int32 v = 0; v < VertsPerFace[f]; v++) {
			if (vOffset < AllVertices.Num()) {
				Vertices.Emplace(AllVertices[vOffset]);
				vOffset++;
			}
		}
		// Add face:
		AddSectionTriangles(VertsPerFace[f], sectionIndex, StartIndex);
		CreateSectionFace(sectionIndex);
		ProcMeshComp->SetMaterial(sectionIndex, GroundSectionMaterial);

		// Reset for next face:
		StartIndex = vOffset;
		sectionIndex++;
		Vertices.Empty();
		Triangles.Empty();
	}
	ProcMeshComp->SetRenderInMainPass(false);
	ProcMeshComp->SetRenderCustomDepth(true);

	KDTree = new CVectorKDTree(SectionMap);
}

bool ACGroundSection::RevealSection(FVector HitLocation)
{
	CurSectionIndex = KDTree->GetNearestNeighbor(HitLocation);
		
	if (CurSectionIndex != PrevSectionIndex) {
		
		if (PrevSectionIndex != -1) {
			ProcMeshComp->SetMeshSectionVisible(PrevSectionIndex, false);
		}
		if (!SectionMap.Find(CurSectionIndex)->bHasWheat) {
			ProcMeshComp->SetMeshSectionVisible(CurSectionIndex, true);
		}
	}
	PrevSectionIndex = CurSectionIndex;

	return true;
}

void ACGroundSection::HideSections()
{
	if (PrevSectionIndex != 1 && CurSectionIndex != -1) {
		ProcMeshComp->SetMeshSectionVisible(CurSectionIndex, false);
		ProcMeshComp->SetMeshSectionVisible(PrevSectionIndex, false);
	}
	PrevSectionIndex = -1;
	CurSectionIndex = -1;
}

void ACGroundSection::PlantAtSection() {
	if (CurSectionIndex != -1) {
		SectionMap.Find(CurSectionIndex)->bHasWheat = true;
		if (WheatComponent) {
			AddWheatInstances(CurSectionIndex);
		}
		HideSections();
	}
}

void ACGroundSection::AddWheatInstances(int32 CurSectionIndex) {
	WheatInfo* Section = SectionMap.Find(CurSectionIndex);
	
	float MinScale = 1.5;
	float MaxScale = 3;
	float Scale = 0;

	for (FVector* Point : Section->DistributedVerts) {
		Scale = FMath::FRandRange(MinScale, MaxScale);
		if (!Section->bIsDirty && InstancePoints.Contains(*Point)) continue;

		int32 NewIndex = WheatComponent->AddInstance(FTransform(Section->SectionNormal, *Point, FVector(Scale, Scale, Scale))); // TODO: Subtract offset here.
		
		if(!Section->bIsDirty)
			InstancePoints.Emplace(*Point);
		Section->NumWheat++;
		WheatInstances.Emplace(CurSectionIndex);

		Section->bIsDirty = false;
		UE_LOG(LogTemp, Warning, TEXT("GROUNDSECTION - Index Value = %d"), NewIndex);
	}
}

void ACGroundSection::CalculateDistributedVerts(int32 SectionIndex){
	//Distribution (TODO: Make Params):
	int32 DesiredNum = 10;

	WheatInfo* Section = SectionMap.Find(SectionIndex);

	int32 NumVerts = Section->Vertices.Num();

	//  Method by triangluating and putting a point at each centroid:
	// Round the Desired Number of points in the polygon down to one obtainable from the aforementioned method.
	// If (Hexagon) = 6*4^x points. If (Pentagon) = 5*4^x points.
	// Calculate x from desired number of points:

	// TODO: Unimplemented:
	int32 NumSubdivisions = FMath::FloorToInt(FMath::Pow(DesiredNum / NumVerts, .4)); // = x. The number of times to subdivide the triangles of the shape provided.

	// Create an instance at vertices(already in list), centroid, and centroids of each triangle:
	Section->DistributedVerts.Emplace(Section->Centroid);
	Section->DistributedVerts.Emplace(GetMidpoint(Section->Vertices[0], Section->Vertices[NumVerts-1]));

	for (int i = 0; i < NumVerts; ++i) {
		Section->DistributedVerts.Emplace(GetMidpoint(Section->Centroid, Section->Vertices[i]));
		if (i < NumVerts - 1) {
			Section->DistributedVerts.Emplace(GetMidpoint(Section->Vertices[i], Section->Vertices[i+1]));
		}
	}
}

FVector * ACGroundSection::GetMidpoint(FVector * P1, FVector * P2)
{
	return new FVector((*P1 + *P2) * 0.5);
}

FQuat ACGroundSection::CalculateNormal(FVector Centroid) {
	// Get the Vector from the center of the planet to the centroid of the section face then convert to FQuat:
	return UKismetMathLibrary::MakeRotFromZY((Centroid - GetActorLocation()), GetActorRightVector()).Quaternion();
}

bool ACGroundSection::RemoveWheatInstance(int32 InstanceIndex, FQuat& Rot) {
	
	int32 SectionIndex = 0;

	if (WheatInstances.Num() > InstanceIndex) {
		SectionIndex = WheatInstances[InstanceIndex];
	}
	else {
		Rot = FQuat::Identity;
		return false;
	}

	WheatInfo* Section = SectionMap.Find(SectionIndex);

	Section->NumWheat--;
	
	WheatComponent->RemoveInstance(InstanceIndex);
	WheatInstances.RemoveAt(InstanceIndex);

	// All instances have been removed in this section:
	if (Section->NumWheat <= 0) {
		Section->bHasWheat = false;
		UE_LOG(LogTemp, Warning, TEXT("GROUNDSECTION - NO WHEAT"));
		Section->bIsDirty = true;
	}

	Rot = Section->SectionNormal;
	return true;
}

ACGroundSection::WheatInfo* ACGroundSection::GetCurrentSection(){
	if (CurSectionIndex != -1) {
		return SectionMap.Find(CurSectionIndex);
	}
	return nullptr;
}
