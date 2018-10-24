#include "CGunTool.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Public/MultiTool/CGunTool.h"
#include "PlantThatWheat.h"

ACGunTool::ACGunTool() {
	MuzzleSocketName = "MuzzleFlashSocket";
	TracerTargetName = "BeamEnd";

	BaseDamage = 20.0f;
}

void ACGunTool::Activate()
{
	Super::Activate();
}

void ACGunTool::Deactivate()
{
	Super::Deactivate();
}

void ACGunTool::Interact()
{
	Super::Interact();
}

void ACGunTool::BeginPlay()
{
	Super::BeginPlay();
}

void ACGunTool::TraceFireEffects(FVector TraceEnd)
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

void ACGunTool::TraceHitEffects(FHitResult const & HitInfo)
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

void ACGunTool::ApplyDamage(AActor * DamagedActor, FVector const & HitFromDirection, FHitResult const & HitInfo, AController * EventInstigator) {
	float ActiveDamage = BaseDamage;

	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitInfo.PhysMaterial.Get());

	if (SurfaceType == SURFACE_FLESHCRITICAL) {
		ActiveDamage *= 4.0f;
	}

	UGameplayStatics::ApplyPointDamage(DamagedActor, ActiveDamage, HitFromDirection, HitInfo, EventInstigator, this, DamageType); // Can be used to specify direction hit for physics sim.
}