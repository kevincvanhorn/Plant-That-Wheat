#include "CPlantingTool.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PlantThatWheat.h"
#include "CPlanetActor.h"
#include "CGroundSection.h"

ACPlantingTool::ACPlantingTool() {
	bCanSingleTrace = true;

	MuzzleSocketName = "MuzzleFlashSocket";
	TracerTargetName = "BeamEnd";

	bCanDamage = false;
	bHasNewFocus = true; // Can focus on a new GroundSection
	bScanForGroundSections = true;
	MaxUseDistance = 800;

	PrimaryActorTick.bCanEverTick = true;
}

void ACPlantingTool::Activate()
{
	Super::Activate();
}

void ACPlantingTool::Deactivate()
{
	if (Planet && Planet->HexGrid) {
		Planet->HexGrid->HideSections();
	}

	Super::Deactivate();
}

void ACPlantingTool::BeginPlay()
{
	Super::BeginPlay();
}

void ACPlantingTool::Interact()
{
	//DoSingleTrace(COLLISION_PLANTINGTOOL);
	UE_LOG(LogTemp, Warning, TEXT("PLANT INTERACT ---------"));
	if (Planet && Planet->HexGrid) {
		Planet->HexGrid->PlantAtSection();
	}
}

void ACPlantingTool::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bScanForGroundSections) {
		DoSingleTrace(COLLISION_PLANTINGTOOL);
		//UE_LOG(LogTemp, Warning, TEXT("GROUND SECTION _---------------------- %s"), (IsGroundSectionInView() ? TEXT("True") : TEXT("False")));
	}
}

bool ACPlantingTool::IsGroundSectionInView()
{
	FVector camLoc;
	FRotator camRot;

	if (OwnerController == NULL)
		return NULL;

	OwnerController->GetActorEyesViewPoint(camLoc, camRot);
	const FVector start_trace = camLoc;
	const FVector direction = camRot.Vector();
	const FVector end_trace = start_trace + (direction * MaxUseDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, start_trace, end_trace, COLLISION_PLANTINGTOOL, TraceParams);

	return Cast<ACPlanetActor>(Hit.GetActor());
}

void ACPlantingTool::OnTraceHit(FHitResult const & HitInfo)
{
	// Reveal the Ground Section at the Trace location:
	Planet = Cast<ACPlanetActor>(HitInfo.GetActor());
	if (Planet && Planet->HexGrid) {
		if (Planet->HexGrid->RevealSection(HitInfo.Location)) {
			// On Successful Reveal.
		}
	}
}

void ACPlantingTool::TraceFireEffects(FVector TraceEnd)
{
}

void ACPlantingTool::TraceHitEffects(FHitResult const & HitInfo)
{
	
}




