#include "CMultiTool.h"
//#include "Engine/World.h" // Not neccessary - used for auto complete.
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlantThatWheat.h"

/* Debug w/ ~WHEAT.DebugTools 1 */
static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugToolDrawing(TEXT("WHEAT.DebugTools"), DebugWeaponDrawing, TEXT("Draw Debug Lines for Tools"), ECVF_Cheat);
// EXEC UPROPERTY value is another way to achieve console debugging.

// Sets default values
ACMultiTool::ACMultiTool()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	bCanTrace = true;
	bShouldTraceOnTick = false;

	SingleTraceDist = 10000;
}

void ACMultiTool::BeginPlay() {
	Super::BeginPlay();
}

void ACMultiTool::TraceFireEffects(FVector TraceEnd)
{
	// Implement this method in subclasses of MultiTool.
}

void ACMultiTool::TraceHitEffects(FHitResult const & HitInfo)
{
}

void ACMultiTool::ApplyDamage(AActor * DamagedActor, FVector const & HitFromDirection, FHitResult const & HitInfo, AController * EventInstigator)
{
	// Implement this method in subclasses of MultiTool.
}

void ACMultiTool::DoSingleTrace()
{
	// Trace from Pawn to center of screen (crosshair).
	AActor* MyOwner = GetOwner();

	if (MyOwner) {
		// Trace Starting Transform
		FVector EyeLocation;
		FRotator EyeRotation; 

		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation); // Fills Eye-prefixed vars w/ data from owning actor.
		FVector TraceDirection = EyeRotation.Vector(); // For Damage functions.
		FVector TraceEnd = EyeLocation + (TraceDirection * SingleTraceDist); // Trace EndLocation

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true; // Trace against each triangle of the mesh - ie. exact location. 
		QueryParams.bReturnPhysicalMaterial = true;	
		 
		FVector TracerEndPoint = TraceEnd; // Effect endpoint paramater.

		FHitResult Hit; // Struct filled with hit data.
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_DEFAULTTOOL, QueryParams)) {
			// Blocking hit, Process damage. 

			AActor * HitActor = Hit.GetActor();
			
			//EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			// Apply Point Damage
			if (bCanDamage) {
				ApplyDamage(HitActor, TraceDirection, Hit, MyOwner->GetInstigatorController());
			}

			TraceHitEffects(Hit); // Impact Effects.

			TracerEndPoint = Hit.ImpactPoint;
		}

		if (DebugWeaponDrawing > 0) {
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f); // Trace Debug Line
		}

		TraceFireEffects(TracerEndPoint);
	}
}
