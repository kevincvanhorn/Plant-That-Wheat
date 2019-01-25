

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWheatManager.generated.h"

class ADirectionalLight;
class AStaticMeshActor;
class ALevelScriptActor;

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
	AStaticMeshActor* TrySpawnWheat(const UObject* WorldContextObject, FTransform SpawnTransform);

	TSubclassOf<AStaticMeshActor> SeedlingClass;

	/* Actors to ignore for planting seedlings. */
	TArray<AActor*> ActorsToIgnore;

protected:
	bool bExistsWheatAtLoc(const FVector Location, UWorld* const World) const;

	bool bIsValidGround(const FVector SpawnLoc, UWorld* const World) const;

	float MinSpawnRadius = 25;

	ACLevelScriptActor* Level;

private:
	float Brightness = 5;
	ADirectionalLight * Sun;
};
