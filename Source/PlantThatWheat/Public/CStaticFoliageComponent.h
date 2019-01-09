

#pragma once

#include "CoreMinimal.h"
#include "FoliageInstancedStaticMeshComponent.h"
#include "CStaticFoliageComponent.generated.h"

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API UCStaticFoliageComponent : public UFoliageInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
	TArray<int32> GetInstancesOverlappingMesh(UStaticMeshComponent& MeshIn) const;

protected:
	bool MeshIntersect(UStaticMeshComponent& MeshIn, const FBox& InstanceBox) const;
};
