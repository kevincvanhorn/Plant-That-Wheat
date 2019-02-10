

#pragma once

#include "CoreMinimal.h"
#include "MultiTool/CMultiTool.h"
#include "CWateringTool.generated.h"

class ACPlanetActor;
class ACDecalActor;

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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector WaterHitLoc;

	UFUNCTION(BlueprintImplementableEvent)
		void OnWaterHitGround();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* Collider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACPlanetActor* Planet;

	/** The number of decals to exist in the watering trail at one moment. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decals")
		int32 NumDecals;

	/** The time (in seconds) between respawning (moving) decals for watering trail. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decals")
		float DecalDelay;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ACDecalActor> DecalClass;

	UFUNCTION(BlueprintImplementableEvent)
		void OnUpdateDecalTrail();

protected:
	void TraceToSurface();

	FVector PlanetLoc;

	UMaterialInstanceDynamic* MatPlanet;

private:
		/* For temporary trail of water left. */
		TArray<ACDecalActor*> DecalArray;
		
		void InitDecalArray();

		UFUNCTION()
		void UpdateDecalTrail();

		//ADecalActor* DecalActor;

		FTimerHandle DecalTrailHandle;

		FRotator WaterHitRot;
	
		int32 CurDecalIndex;
};
