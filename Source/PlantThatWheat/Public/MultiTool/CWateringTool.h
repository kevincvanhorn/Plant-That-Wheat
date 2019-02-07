

#pragma once

#include "CoreMinimal.h"
#include "MultiTool/CMultiTool.h"
#include "CWateringTool.generated.h"

class ACPlanetActor;

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACWateringTool : public ACMultiTool
{
	GENERATED_BODY()

	
public:
	ACWateringTool();

	void Init(ACPlanetActor* Planet);

	virtual void Activate() override;

	virtual void Deactivate() override;

	virtual void BeginPlay() override;

	virtual void Interact() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector WaterHitLoc;

	UFUNCTION(BlueprintImplementableEvent)
		void OnWaterHitGround();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* Collider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACPlanetActor* Planet;

protected:
	void TraceToSurface();

	FVector PlanetLoc;

	UMaterialInstanceDynamic* MatPlanet;
};
