#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CUsableActor.h"
#include "CGroundSection.generated.h"

class UProceduralMeshComponent;
class ACCharacterBase;
class CVectorKDTree;
class UHierarchicalInstancedStaticMeshComponent;
class UStaticMesh;

UCLASS()
class PLANTTHATWHEAT_API ACGroundSection : public AActor
{
	GENERATED_BODY()
	
public:	
	struct WheatInfo {
		FVector* Centroid;
		bool bHasWheat;
		TArray<FVector*> Vertices;
		TArray<FVector*> DistributedVerts;
		FQuat SectionNormal;
	};

	// Sets default values for this actor's properties
	ACGroundSection();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static ACGroundSection* CREATE(const UObject* WorldContextObject, FTransform SpawnTransform, TArray<FVector> Vertices);
	
	static ACGroundSection* CREATE(const UObject* WorldContextObject, FTransform SpawnTransform, TArray<FVector> AllVertices, TArray<int32> VertsPerFace, UMaterial* GroundSectionMaterial, float HexGridScale, UStaticMesh* WheatMesh);

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * ProcMeshComp;

	UPROPERTY(VisibleAnywhere)
	UHierarchicalInstancedStaticMeshComponent* WheatComponent;

	UPROPERTY(VisibleAnywhere)
		UStaticMesh* WheatMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Ground Section")
	UPhysicalMaterial *SurfaceType;

	TArray<FVector> Vertices; // For one face section

	TArray<FVector> AllVertices;
	TArray<FVector> AllTriangles;
	TArray<int32> VertsPerFace; // Used when creating entire hex grid from array.

	/** Attempt to reveal a mesh section near a hit location. */
	bool RevealSection(FVector HitLocation);

	UMaterial* GroundSectionMaterial;

	void HideSections();

	void PlantAtSection();

/* Initialization Methods: */
private:
	void PostActorCreated();
	void PostLoad();

protected:
	TArray<int32> Triangles;

	void PreSpawnInitialize(TArray<FVector> Vertices);
	void PreSpawnInitialize(TArray<FVector> AllVertices, TArray<int32> VertsPerFace);

	// Map of Vectors to section indices:
	//TMap<int32, FVector> SectionMap;

	TMap<int32, WheatInfo> SectionMap; 

private:
	void AddSectionTriangles(int32 NumVerts, int32 sectionIndex, int32 StartIndex);
	//void AddSectionTriangles(int32 NumVerts, int32 sectionIndex, int32 StartIndex);
	void CreateSectionFace(int32 sectionIndex);
	void CreateAllSections();

	/** Create one procMesh and then faces as subsections if true*/
	bool bCreateSubsections; 

	CVectorKDTree* KDTree;

	int32 CurSectionIndex = -1;
	int32 PrevSectionIndex = -1;

	float HexGridScale;


	void CalculateDistributedVerts(int32 SectionIndex); // Pre-compute points within the section to spawn foliage. 
	FQuat* CalculateNormal(TArray<FVector*> Vertices);
	FQuat CalculateNormal(FVector Centroid);
	FRotator* CalculateNormal(int32 SectionIndex);
	void AddWheatInstances(int32 CurSectionIndex);

	FVector* GetCentroid(FVector * P1, FVector * P2);

	TSet<FVector*> InstancePoints;
};
