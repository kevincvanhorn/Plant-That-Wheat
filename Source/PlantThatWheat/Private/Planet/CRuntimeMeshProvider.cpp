#include "CRuntimeMeshProvider.h"

FVector UCRuntimeMeshProvider::GetBoxRadius() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return BoxRadius;
}

void UCRuntimeMeshProvider::SetBoxRadius(const FVector& InRadius)
{
	FScopeLock Lock(&PropertySyncRoot);
	BoxRadius = InRadius;
}

UMaterialInterface* UCRuntimeMeshProvider::GetBoxMaterial() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return Material;
}

void UCRuntimeMeshProvider::SetBoxMaterial(UMaterialInterface* InMaterial)
{
	FScopeLock Lock(&PropertySyncRoot);
	Material = InMaterial;
}