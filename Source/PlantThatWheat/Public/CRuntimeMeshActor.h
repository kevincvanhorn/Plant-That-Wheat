

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshActor.h"
#include "CRuntimeMeshActor.generated.h"

UCLASS()
class PLANTTHATWHEAT_API ACRuntimeMeshActor : public ARuntimeMeshActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

public:
	ACRuntimeMeshActor();

	void OnConstruction(const FTransform& Transform) override;

};
