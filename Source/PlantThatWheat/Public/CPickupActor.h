

#pragma once

#include "CoreMinimal.h"
#include "CUsableActor.h"
#include "CPickupActor.generated.h"

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACPickupActor : public ACUsableActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComp;	
	
};
