

#pragma once

#include "CoreMinimal.h"
#include "CRuntimeMeshActor.generated.h"

UCLASS()
class PLANTTHATWHEAT_API ACRuntimeMeshActor : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

public:
	ACRuntimeMeshActor();

};
