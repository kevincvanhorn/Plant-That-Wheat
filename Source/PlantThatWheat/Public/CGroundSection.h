#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CUsableActor.h"
#include "CGroundSection.generated.h"

class UProceduralMeshComponent;
class ACCharacterBase;

UCLASS()
class PLANTTHATWHEAT_API ACGroundSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACGroundSection();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static ACGroundSection* CREATE(const UObject* WorldContextObject, FTransform SpawnTransform, TArray<FVector> Vertices);
	
	static ACGroundSection* CREATE(const UObject* WorldContextObject, FTransform SpawnTransform, TArray<FVector> AllVertices, TArray<int32> VertsPerFace);

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * ProcMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Ground Section")
	UPhysicalMaterial *SurfaceType;

	TArray<FVector> Vertices; // For one face section

	TArray<FVector> AllVertices;
	TArray<int32> VertsPerFace; // Used when creating entire hex grid from array.

	/** Attempt to reveal a mesh section near a hit location. */
	bool RevealSection(FVector HitLocation);

/* Initialization Methods: */
private:
	void PostActorCreated();
	void PostLoad();

protected:
	TArray<int32> Triangles;

	void PreSpawnInitialize(TArray<FVector> Vertices);
	void PreSpawnInitialize(TArray<FVector> AllVertices, TArray<int32> VertsPerFace);

	// Map of Vectors to section indices:
	TMap<int32, FVector> SectionMap;

private:
	void AddSectionTriangles(int32 NumVerts, int32 sectionIndex);
	void CreateSectionFace(int32 sectionIndex);
	void CreateAllSections();

	/** Create one procMesh and then faces as subsections if true*/
	bool bCreateSubsections; 
};
