

#include "CMultiTool.h"
//#include "Engine/World.h" // Not neccessary - used for auto complete.
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PlantThatWheat.h"
#include "Runtime/Foliage/Public/InstancedFoliageActor.h"
#include "EngineUtils.h" // ActorIterator.

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

	BaseDamage = 20.0f;
}

void ACMultiTool::BeginPlay() {
	Super::BeginPlay();

	TActorIterator<AInstancedFoliageActor> foliageIterator(GetWorld());
	AInstancedFoliageActor* foliageActor = *foliageIterator;

	//if you already have foliage in your level, you just need to get the right component, one is created for each type
	TArray<UInstancedStaticMeshComponent*> components;
	foliageActor->GetComponents<UInstancedStaticMeshComponent>(components);
	if (components.Num() > 0)
		foliageMeshComponent = components[0];
		
	//If you havent, you need to create the instanced static mesh component too
	//else
	//{
	//foliageMeshComponent = NewObject<UInstancedStaticMeshComponent>(foliageActor, UInstancedStaticMeshComponent::StaticClass(), NAME_None, RF_Transactional);
	//foliageMeshComponent->AttachToComponent(foliageActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	//foliageMeshComponent->SetStaticMesh(PlantAsset);
	//foliageMeshComponent->RegisterComponent();
	//}
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
		QueryParams.bReturnPhysicalMaterial = true;	

		// Particle "Target" paramater. 
		FVector TracerEndPoint = TraceEnd; 

		FHitResult Hit; // Struct filled with hit data.
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_MULTITOOL, QueryParams)) { // Channel = anything visible in the world.
			// Blocking hit, Process damage. 

			AActor * HitActor = Hit.GetActor();
			
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
			
			float ActiveDamage = BaseDamage;

			if (SurfaceType == SURFACE_FLESHCRITICAL) {
				ActiveDamage *= 4.0f;
			}

			UGameplayStatics::ApplyPointDamage(HitActor, ActiveDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType); // Can be used to specify direction hit for physics sim.

			/* Impact Effect: */
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
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation()); // Could use inverse of shot direction over impact normal for diff effect.
			}

			TracerEndPoint = Hit.ImpactPoint;

			PlantOnHit(TracerEndPoint);
		}

		if (DebugWeaponDrawing > 0) {
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f); // Trace Debug Line
		}

		PlayFireEffects(TracerEndPoint);
		PlantOnHit(TracerEndPoint);
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

	/* Camera Shake: */
	APawn * MyOwner = Cast<APawn>(GetOwner());	
	if (MyOwner) {
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC) {
			PC->ClientPlayCameraShake(FireCameraShake);
		}
	}
}

void ACMultiTool::PlantOnHit(FVector TraceEnd) {
	if (PlantAsset) {
		FTransform transform = FTransform();
		transform.SetLocation(TraceEnd);
		//GetWorld()->SpawnActor(PlantAsset, &TraceEnd);
		foliageMeshComponent->AddInstance(transform);

	}
}
