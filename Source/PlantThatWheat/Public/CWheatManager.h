

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWheatManager.generated.h"

class ADirectionalLight;
//class AStaticMeshActor;
class ALevelScriptActor;

class ACWheatSpawnable;
class UCStaticFoliageComponent;

UCLASS()
class PLANTTHATWHEAT_API ACWheatManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACWheatManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	ACWheatSpawnable* TrySpawnWheat(const UObject* WorldContextObject, FTransform SpawnTransform, EPhysicalSurface SurfaceType);

	TSubclassOf<ACWheatSpawnable> SeedlingClass;

	/* Actors to ignore for planting seedlings. */
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(VisibleAnywhere)
		UCStaticFoliageComponent* FoliageComponent_Dead;

	UPROPERTY(VisibleAnywhere)
		UStaticMesh* WheatMesh_Dead;

	bool AddInstance_Dead(FTransform & Transform);

	bool SetFoliageMesh_Dead(UStaticMesh* Mesh);

protected:
	bool bExistsWheatAtLoc(const FVector Location, UWorld* const World) const;

	bool bIsValidGround(const FVector SpawnLoc, UWorld* const World, EPhysicalSurface SurfaceType) const;

	float MinSpawnRadius = 25;

	ACLevelScriptActor* Level;

private:
	float Brightness = 5;
	ADirectionalLight * Sun;
};
