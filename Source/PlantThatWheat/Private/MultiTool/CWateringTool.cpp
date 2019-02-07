

#include "CWateringTool.h"
#include "PlantThatWheat.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

#include "CPlanetActor.h"

#include "Engine/Classes/Materials/MaterialInstanceDynamic.h"


ACWateringTool::ACWateringTool() {

	bCanSingleTrace = false;
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collider"));
	if (Collider) {
		Collider->SetupAttachment(RootComponent);
	}

	WaterHitLoc = FVector::ZeroVector;
}

void ACWateringTool::Init(ACPlanetActor* Planet)
{
	this->Planet = Planet;
	this->PlanetLoc = Planet->GetActorLocation();

	//UMaterialInterface* MPlanet = Planet->MeshComponent->GetMaterial(0);
	this->MatPlanet = Planet->DynamicMaterial;
}

void ACWateringTool::Activate()
{
	Super::Activate();
}

void ACWateringTool::Deactivate()
{
	Super::Deactivate();
}

void ACWateringTool::BeginPlay()
{
	Super::BeginPlay();
}

void ACWateringTool::Interact()
{

}

// Not called when tool is deactivated:
void ACWateringTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceToSurface();

	if (MatPlanet->IsValidLowLevel()) {
		// Set Material
		MatPlanet->SetVectorParameterValue("WaterLoc", WaterHitLoc);
	}
}

void ACWateringTool::TraceToSurface()
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true; // Trace against each triangle of the mesh - ie. exact location. 
	QueryParams.bReturnPhysicalMaterial = true;

	FVector TraceStart = Collider->GetComponentLocation();
	FVector TraceDirection = (PlanetLoc - TraceStart).GetSafeNormal(); // TraceStart -> PlanetCenter 
	TraceStart += TraceDirection * -1 * 200; // Move up by normal so that the planet is always hit.
	FVector TraceEnd = TraceStart + (TraceDirection * SingleTraceDist); // Trace EndLocation

	FHitResult Hit; // Struct filled with hit data.
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Cyan, false, 2);
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, COLLISION_DIGTRACE, QueryParams)) {
		WaterHitLoc = Hit.Location;
	}
}
