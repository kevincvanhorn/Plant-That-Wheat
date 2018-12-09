#pragma once

#include "CoreMinimal.h"
#include "CustomGravity/PlanetActor.h"
#include "CPlanetActor.generated.h"

class ACGroundSection;
class ACPlanetProceduralMesh;
class UMaterial;
class UProceduralMeshComponent;
class ACCapture;

UCLASS()
class PLANTTHATWHEAT_API ACPlanetActor : public APlanetActor
{
	GENERATED_BODY()
	
public:

	ACPlanetActor(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	/** Size of the static mesh base of the planet actor. */
	UPROPERTY(EditDefaultsOnly, Category = "PlanetActor")
	FVector StaticMeshScale;

	/** Planet Procedural Grid Array: */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlanetActor") // 
	TArray<TSubclassOf<ACGroundSection>> GroundGrid;

	UPROPERTY(EditAnywhere, Category = "PlanetActor")
		UMaterial* GroundSectionMaterial;

	UPROPERTY(EditAnywhere, Category = "PlanetActor")
	UMaterial* StoredMaterial;

	UPROPERTY(EditAnywhere, Category = "PlanetActor")
	TSubclassOf<ACCapture> CaptureCompClass;

	ACCapture *CaptureComp;

private:
	ACPlanetProceduralMesh* ProcBoundingMesh;
	UMaterialInstanceDynamic* DynamicMaterial;
};
