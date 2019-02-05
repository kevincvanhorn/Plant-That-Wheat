

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CLevelScriptActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollectWheat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollectAllWheat);


class ADirectionalLight;
class ACWheatManager;
class ACPlanetActor;

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ACLevelScriptActor();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "Objectives")
		FOnCollectWheat OnCollectWheat;

	UPROPERTY(BlueprintAssignable, Category = "Objectives")
		FOnCollectAllWheat OnCollectAllWheat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Lighting)
	ADirectionalLight * Sun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Planet)
		ACPlanetActor* Planet;

	/* Store the location of a new hole sphere mask from the shovel tool. */
	void AddDigArea(FVector DigCentroid, float HoleRadius);

	/* Check if a point is within one of the dig areas. */
	bool bWithinDigArea(FVector& QueryLoc);

protected:
	TSet<FSphere*> PlanetHoles;
};
