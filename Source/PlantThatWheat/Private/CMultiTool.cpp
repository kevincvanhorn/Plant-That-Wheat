// Fill out your copyright notice in the Description page of Project Settings.

#include "CMultiTool.h"
//#include "Engine/World.h" // Not neccessary - used for auto complete.
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

/* Debug w/ ~WHEAT.DebugWeapons 1 */
static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(TEXT("WHEAT.DebugWeapons"), DebugWeaponDrawing, TEXT("Draw Debug Lines for Weapons"), ECVF_Cheat);

// Sets default values
ACMultiTool::ACMultiTool()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleFlashSocket";
	TracerTargetName = "BeamEnd";
}


void ACMultiTool::Fire()
{
	// Trace from Pawn to center of screen (crosshair).

	AActor* MyOwner = GetOwner();
	if (MyOwner) {
		FVector EyeLocation;
		FRotator EyeRotation; // Trace Start Location

		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);		// Fills Eye-vars by ref w/ data from owning actor.

		FVector ShotDirection = EyeRotation.Vector(); // For Damage functions.

		FVector TraceEnd = EyeLocation + (ShotDirection * 10000); // Trace EndLocation

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true; // Trace against each triangle of the mesh - ie. exact location. 

		// Particle "Target" paramater. 
		FVector TracerEndPoint = TraceEnd; 

		FHitResult Hit; // Struct filled with hit data.
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams)) { // Channel = anything visible in the world.
			// Blocking hit, Process damage. 

			AActor * HitActor = Hit.GetActor();
			UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType); // Can be used to specify direction hit for physics sim.

			/* Impact Effect: */
			if (ImpactEffect) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation()); // Could use inverse of shot direction over impact normal for diff effect.
			}

			TracerEndPoint = Hit.ImpactPoint;
		}

		if (DebugWeaponDrawing > 0) {
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f); // Trace Debug Line
		}

		PlayFireEffects(TracerEndPoint);
		
	}
}

void ACMultiTool::PlayFireEffects(FVector TraceEnd)
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
}
