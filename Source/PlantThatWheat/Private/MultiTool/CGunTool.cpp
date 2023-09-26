#include "CGunTool.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

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
		}
	}
}

void ACGunTool::TraceHitEffects(FHitResult const & HitInfo)
{
}

void ACGunTool::ApplyDamage(AActor * DamagedActor, FVector const & HitFromDirection, FHitResult const & HitInfo, AController * EventInstigator) {
	float ActiveDamage = BaseDamage;

	UGameplayStatics::ApplyPointDamage(DamagedActor, ActiveDamage, HitFromDirection, HitInfo, EventInstigator, this, DamageType); // Can be used to specify direction hit for physics sim.
}