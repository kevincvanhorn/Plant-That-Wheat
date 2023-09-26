

#pragma once

#include "CoreMinimal.h"
#include "CRuntimeMeshProvider.generated.h"

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API UCRuntimeMeshProvider : public UActorComponent
{
	GENERATED_BODY()
	
private:
	mutable FCriticalSection PropertySyncRoot;

	UPROPERTY(Category = "RuntimeMesh|Providers|Box", VisibleAnywhere, BlueprintGetter=GetBoxRadius, BlueprintSetter=SetBoxRadius)
	FVector BoxRadius;

	UPROPERTY(Category = "RuntimeMesh|Providers|Box", VisibleAnywhere, BlueprintGetter=GetBoxMaterial, BlueprintSetter=SetBoxMaterial)
	UMaterialInterface* Material;
public:
	UFUNCTION(Category = "RuntimeMesh|Providers|Box", BlueprintCallable)
	FVector GetBoxRadius() const;
	UFUNCTION(Category = "RuntimeMesh|Providers|Box", BlueprintCallable)
	void SetBoxRadius(const FVector& InRadius);

	UFUNCTION(Category = "RuntimeMesh|Providers|Box", BlueprintCallable)
	UMaterialInterface* GetBoxMaterial() const;
	UFUNCTION(Category = "RuntimeMesh|Providers|Box", BlueprintCallable)
	void SetBoxMaterial(UMaterialInterface* InMaterial);
};
