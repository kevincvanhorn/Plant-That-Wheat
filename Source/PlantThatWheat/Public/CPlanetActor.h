

#pragma once

#include "CoreMinimal.h"
#include "CustomGravity/PlanetActor.h"
#include "CPlanetActor.generated.h"


class ACGroundSection;

UCLASS()
class PLANTTHATWHEAT_API ACPlanetActor : public APlanetActor
{
	GENERATED_BODY()
	
public:
	/** Planet Procedural Grid Array: */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlanetActor") // 
	TArray<TSubclassOf<ACGroundSection>> GroundGrid;
};
