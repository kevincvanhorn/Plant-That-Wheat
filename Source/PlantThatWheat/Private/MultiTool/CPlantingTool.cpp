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
}

void ACPlantingTool::Activate()
{
	Super::Activate();
}

void ACPlantingTool::Deactivate()
{
	Super::Deactivate();
}

void ACPlantingTool::BeginPlay()
{
	Super::BeginPlay();
}

void ACPlantingTool::Interact()
{
	DoSingleTrace(COLLISION_PLANTINGTOOL);

	UE_LOG(LogTemp, Warning, TEXT("PLANT INTERACT ---------"));
}

void ACPlantingTool::OnTraceHit(FHitResult const & HitInfo)
{
	Super::OnTraceHit(HitInfo); // Play Effects.

	// Reveal the Ground Section at the Trace location:
	ACPlanetActor* Planet = Cast<ACPlanetActor>(HitInfo.GetActor());
	if (Planet && Planet->HexGrid) {
		if (Planet->HexGrid->RevealSection(HitInfo.Location)) {
			// On Successful Reveal.
		}
	}
}

void ACPlantingTool::TraceFireEffects(FVector TraceEnd)
{
	/* Muzzle Effect: */
	if (MuzzleEffect) {
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName); // FName
	}

	/* Tracer Effect: */
	if (TracerEffect) {
		FVector  MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		UParticleSystemComponent * TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp) {
			TracerComp->SetVectorParameter(TracerTargetName, TraceEnd);
		}
	}

	/* Camera Shake: */
	APawn * MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner) {
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC) {
			PC->ClientPlayCameraShake(FireCameraShake);
		}
	}
}

void ACPlantingTool::TraceHitEffects(FHitResult const & HitInfo)
{
	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitInfo.PhysMaterial.Get());

	UParticleSystem* SelectedEffect = nullptr;
	switch (SurfaceType) {
	case SURFACE_FLESHDEFAULT: // Fall through to execute code for next case until break.
	case SURFACE_FLESHCRITICAL:
		SelectedEffect = FleshImpactEffect;
		break;
	default:
		SelectedEffect = DefaultImpactEffect;
		break;
	}

	if (SelectedEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, HitInfo.ImpactPoint, HitInfo.ImpactNormal.Rotation()); // Could use inverse of shot direction over impact normal for diff effect.
	}
}




