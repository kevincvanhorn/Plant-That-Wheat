

#include "CPlanetActor.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Engine/StaticMesh.h"
#include "CPlanetProceduralMesh.h"

// ObjectInitializer Constructor used intstead of ACPlanetActor() for PlanetActor Constructor compatibility.
ACPlanetActor::ACPlanetActor(const FObjectInitializer& Objectinititializer) {

	// Planet Mesh scale = Transform.Scale * PlanetMeshScale * PlanetMesh.ApproxSize;

	// GroundGrid.Add() GroundSection Elements from HexVerts.

	//ProcBoundingMesh = CreateDefaultSubobject<ACPlanetProceduralMesh>(TEXT("ProcBoundingMesh"));

	//const FTransform SpawnTransform();
	/* Called before BeginPlay() */
	/*auto const MyActor = GetWorld()->SpawnActorDeferred<ACPlanetProceduralMesh>(ACPlanetProceduralMesh::StaticClass(), GetTransform());
	if (MyActor != nullptr)
	{
		MyActor->PreSpawnInitialize();
		MyActor->FinishSpawning(GetTransform());
	}*/

	
}

void ACPlanetActor::BeginPlay() {
	Super::BeginPlay();


	// Planet Mesh scale = Transform.Scale * PlanetMeshScale * PlanetMesh.ApproxSize;
	FTransform AdjustedTransform = GetTransform() * PlanetMeshScale * PlanetMesh->getB; //TODO

	UE_LOG(LogTemp, Warning, TEXT("%s"), *TransformedVector.ToString());

	//FTransform AdjustedTransform = GetTransform();
	//AdjustedTransform.SetScale3D(FVector(4000, 4000, 4000));

	if (GetWorld() != NULL) {
		ProcBoundingMesh = ACPlanetProceduralMesh::CREATE(GetWorld(), AdjustedTransform, true, true);
	}
}