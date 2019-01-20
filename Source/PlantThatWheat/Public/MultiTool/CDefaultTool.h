/* Default Mode - place & pick up. */

#pragma once

#include "CoreMinimal.h"
#include "CMultiTool.h"
#include "CDefaultTool.generated.h"


class ACUsableActor;
class ACMoveableActor;
class ACGroundSection;

UCLASS()
class PLANTTHATWHEAT_API ACDefaultTool : public ACMultiTool
{
	GENERATED_BODY()
	
public:
	ACDefaultTool();

	virtual void Tick(float DeltaSeconds) override;

	virtual void Activate() override;

	virtual void Deactivate() override;

	/** Use the UsableActor if in focus. */
	virtual void Interact();

	bool bIsHoldingMoveable;

	/** Should the tool trace for UsableActors every tick? */
	bool bScanForUsables;

protected:
	/* Max distance to use/focus on actors. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MultiTool")
		float MaxUseDistance;

	/** Check for and return the UsableActor that the player is looking at - nullptr if none. */
	class ACUsableActor* GetUsableInView();

	/* True only in first frame when focused on new usable actor. */
	bool bHasNewFocus;

	/* Actor derived from UsableActor currently in center-view - nullptr if none. */
	ACUsableActor* FocusedUsableActor;
	
private:
	ACMoveableActor *MoveableActor;

	ACGroundSection *GroundSection;
};
