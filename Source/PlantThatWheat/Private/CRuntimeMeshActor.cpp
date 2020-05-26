#include "CRuntimeMeshActor.h"
#include "RuntimeMeshComponent.h"

#include "Providers/RuntimeMeshProviderBox.h"
#include "RuntimeMeshProvider.h"

#include "CRuntimeMeshProvider.h"


ACRuntimeMeshActor::ACRuntimeMeshActor()
{
}

void ACRuntimeMeshActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UCRuntimeMeshProvider* Provider = NewObject<UCRuntimeMeshProvider>(this, TEXT("CProvider"));
	if (Provider) {
		Provider->SetBoxRadius(FVector(100, 100, 100));
		Provider->SetBoxMaterial(Material);
		GetRuntimeMeshComponent()->Initialize(Provider);
	}

	/*
	URuntimeMeshProviderBox* BoxProvider = NewObject<URuntimeMeshProviderBox>(this, TEXT("RuntimeMeshProvider-Box"));
	if (BoxProvider)
	{
		BoxProvider->SetBoxRadius(FVector(100, 100, 100));
		BoxProvider->SetBoxMaterial(Material);
		GetRuntimeMeshComponent()->Initialize(BoxProvider);
	}*/
}