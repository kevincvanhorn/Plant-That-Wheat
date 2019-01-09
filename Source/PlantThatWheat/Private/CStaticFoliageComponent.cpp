

#include "CStaticFoliageComponent.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"


TArray<int32> UCStaticFoliageComponent::GetInstancesOverlappingMesh(UStaticMeshComponent& MeshIn) const
{
	TArray<int32> Result;

	if (UStaticMesh* Mesh = GetStaticMesh())
	{
		const FVector StaticMeshBoundsExtent = Mesh->GetBounds().BoxExtent;

		for (int32 Index = 0; Index < PerInstanceSMData.Num(); Index++)
		{
			const FMatrix& Matrix = PerInstanceSMData[Index].Transform;
			FBox InstanceBox(Matrix.GetOrigin() - StaticMeshBoundsExtent, Matrix.GetOrigin() + StaticMeshBoundsExtent);

			if (MeshIntersect(MeshIn, InstanceBox))
			{
				Result.Add(Index);
			}
		}
	}

	return Result;
}

bool UCStaticFoliageComponent::MeshIntersect(UStaticMeshComponent& MeshIn, const FBox& InstanceBox) const
{
	FCollisionShape QueryBox = FCollisionShape::MakeBox(InstanceBox.GetExtent());
	return MeshIn.OverlapComponent(InstanceBox.GetCenter(), FQuat::Identity, QueryBox);

	// Check if the min or max point is in the collision:
	//return (MeshIn.GetDistanceToCollision(InstanceBox.Min, RefVector) == 0) && (MeshIn.GetDistanceToCollision(InstanceBox.Max, RefVector) == 0);
}