// https://en.wikipedia.org/wiki/List_of_uniform_polyhedra
#pragma once

#include "CoreMinimal.h"
#include "CustomGravity/PlanetActor.h"
#include "CPlanetActor.generated.h"

class ACGroundSection;
class ACPlanetProceduralMesh;
class UMaterial;
class UProceduralMeshComponent;
class ACCapture;
class UHierarchicalInstancedStaticMeshComponent;
class UStaticMesh;


UCLASS()
class PLANTTHATWHEAT_API ACPlanetActor : public APlanetActor
{
	GENERATED_BODY()
	
public:
	ACPlanetActor(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	/** Size of the static mesh base of the planet actor. */
	UPROPERTY(EditAnywhere, Category = "PlanetActor")
	float StaticMeshScale;

	/** Size of the static mesh base of the planet actor. */
	UPROPERTY(EditAnywhere, Category = "PlanetActor")
		float HexGridOffset;

	/** Planet Procedural Grid Array: */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlanetActor") // 
	TArray<TSubclassOf<ACGroundSection>> GroundGrid;

	UPROPERTY(EditAnywhere, Category = "PlanetActor")
		UMaterial* GroundSectionMaterial;

	UPROPERTY(EditAnywhere, Category = "PlanetActor")
		UStaticMesh* WheatComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlanetActor")
	UMaterial* StoredMaterial;

	UPROPERTY(EditAnywhere, Category = "PlanetActor")
	TSubclassOf<ACCapture> CaptureCompClass;

	UPROPERTY(EditAnywhere, Category = "PlanetActor")
		bool bUseCaptureComponent;

	ACCapture *CaptureComp;

	ACGroundSection *HexGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* DynamicMaterial;

private:
	ACPlanetProceduralMesh* ProcBoundingMesh;

	float HexGridScale;

	void InitCaptureComponent();

	void InitHexGrid();
};
