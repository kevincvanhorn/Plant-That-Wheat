

#pragma once

#include "CoreMinimal.h"
#include "Objects/CMoveableActor.h"
#include "CUmbrellaMoveable.generated.h"

class ADecalActor;
class FViewport;
class APlayerController;
class ACMultiTool;

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACUmbrellaMoveable : public ACMoveableActor
{
	GENERATED_BODY()

public:
	ACUmbrellaMoveable();

	virtual void BeginPlay() override;

	virtual bool OnUsed_Implementation(ACMultiTool * Tool) override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnFinishManualRot() override;

protected:
	virtual void PickUp() override;

	virtual void SetDown() override;

	void TraceToSurface();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADecalActor> DecalClass;

	ADecalActor* PlacementDecal;

	FTimerHandle FUmbrellaPlaceHandle;

	float DistToDecal;
	float SpeedToDecal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TimeToDecal = 2; // How long it should take to move the umbrella to the ground.

	UFUNCTION()
	void MoveToSurface();

	FViewport* Viewport;

	APlayerController* PlayerController;

	/* Dampen the speed of rotation by this amount. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MouseRotFactor = 0.5f;

private:
	float TimeSincePlace;

	bool bCanManualRotate;
	FRotator MeshRot; // For mouse rotation from this offset

	/* Takes a given euler reference angle and clamps an input angle by a given DOF on both sides.*/
	float ClampFromReferenceRot(float &ClampedAngle, float RefAngle, float DOF);
};
