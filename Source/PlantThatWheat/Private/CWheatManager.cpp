

#include "CWheatManager.h"
#include "Kismet/KismetMathLibrary.h" 
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"

#include "GameFramework/Actor.h"


#include "CWheatSpawnable.h"

#include "CLevelScriptActor.h"
#include "Engine/DirectionalLight.h"

#include "PlantThatWheat.h"

// Sets default values
ACWheatManager::ACWheatManager()
{

}

// Called when the game starts or when spawned
void ACWheatManager::BeginPlay()
{
	Super::BeginPlay();


	float EnvironmentIntensity = 5; // [0 -> 10]
	float SunIntensity = 4; // [0 -> 20]

	// Set Light limits:
	Level = Cast<ACLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	if (Level) {
		Sun = Level->Sun;
		if (Sun) {
			//Sun->GetLightComponent()
			SunIntensity = Sun->GetBrightness();
			UE_LOG(LogTemp, Warning, TEXT("SUN --------- %f"), SunIntensity);

		}
	}
	UWorld* World = GetWorld();
	if (World) {
		EnvironmentIntensity = World->GetWorldSettings()->LightmassSettings.EnvironmentIntensity; // 6 - 10
	}

	// Let combined environment and light intensity at 10 be defined as very bright - e.g. scorching desert.
	Brightness = EnvironmentIntensity + SunIntensity; // 9 in desert
}

ACWheatSpawnable * ACWheatManager::TrySpawnWheat(const UObject * WorldContextObject, FTransform SpawnTransform, EPhysicalSurface SurfaceType)
{
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);

	if (SeedlingClass && World) {
		if (!bExistsWheatAtLoc(SpawnTransform.GetLocation(), World)) {
			if (bIsValidGround(SpawnTransform.GetLocation(), World, SurfaceType)) {
				ACWheatSpawnable* Spawnable =  World->SpawnActor<ACWheatSpawnable>(SeedlingClass, SpawnTransform.GetLocation(), SpawnTransform.Rotator(), ActorSpawnParams);
				Spawnable->SpawnHealthy();
				return Spawnable;
			}
			else {
				// Shrivel
				ACWheatSpawnable* Spawnable = World->SpawnActor<ACWheatSpawnable>(SeedlingClass, SpawnTransform.GetLocation(), SpawnTransform.Rotator(), ActorSpawnParams);
				Spawnable->SpawnUnhealthy();
				return Spawnable;
			}
		}
	}
	return nullptr;
}



bool ACWheatManager::bExistsWheatAtLoc(const FVector Location, UWorld* const World) const
{
	if (!World ) return false;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic) };
	TArray<AActor*> OutActors;

	bool Result = UKismetSystemLibrary::SphereOverlapActors(World, Location, MinSpawnRadius, ObjectTypes, SeedlingClass, ActorsToIgnore, OutActors);

	//DrawDebugSphere(World, Location, MinSpawnRadius, 10, FColor::Red, false, 10);

	for (auto Elem : OutActors) {
		UE_LOG(LogTemp, Warning, TEXT("WHEAT: Overlap Actors --------- %s"), *GetDebugName(Elem));
	}

	return Result;
}

bool ACWheatManager::bIsValidGround(const FVector SpawnLoc, UWorld * const World, EPhysicalSurface SurfaceType) const
{
	if(!World) return false;

	// Check Surface types (e.g. can't plant on sand)
	if (SurfaceType != SURFACE_GROUND) {
		return false;
	}

	// Check Sunlight is not too strong:
	if (Sun && Brightness >= 7) {
		FVector VTowardSun = Sun->GetActorRotation().Vector() * -1; // Unit vector toward the sun

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FVector TraceEnd = SpawnLoc + VTowardSun * 1000;

		FHitResult HitResult; // Struct filled with hit data.
		DrawDebugLine(World, SpawnLoc, TraceEnd, FColor::Red, false, 2);

		// Objects that should block sunlight must LIGHTTRACE Channel enabled (blocking by default).
		if (World->LineTraceSingleByChannel(HitResult, SpawnLoc, TraceEnd, COLLISION_LIGHTTRACE, QueryParams)) {
			UE_LOG(LogTemp, Warning, TEXT("WHEAT: Trace Hit, %s"), *HitResult.ToString());
			return true;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("WHEAT: Trace No Hit"));
			return false;
		}
	}
	return true;
}
