#pragma once

#include "CoreMinimal.h"
#include "CUsableActor.h"
#include "CustomPhysicsActor.h"
#include "CMoveableActor.generated.h"

class ACCharacterBase;
class ACMultiTool;

UCLASS()
class PLANTTHATWHEAT_API ACMoveableActor : public ACustomPhysicsActor
{
	GENERATED_BODY()

public:
	ACMoveableActor();

	virtual void Tick(float DeltaTime) override;

	/** The x (Right) y (Forward) z (Up) offset from the transform of the Player. */
	UPROPERTY(EditAnywhere, Category = "Moveable Actor")
	FVector PawnOffset;

	virtual bool OnUsed_Implementation(ACMultiTool * Tool) override;

	virtual bool StartFocus_Implementation() override;

	virtual bool EndFocus_Implementation() override;

	void SetValidToolMode(bool InValue) { bValidToolMode = InValue; }

	/** If the Moveable Actor supports outlines, disable Custom Depth rendering. */
	void DisableOutlines();

	bool bCanAffectObjective = true;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnObjectiveOverlap();

	UFUNCTION(BlueprintCallable)
		void External_DropMoveable();
	
protected:

	bool bIsBeingHeld;

	void PickUp();

	void SetDown();

	FVector ForwardVector;

	FVector HoldLocation;

	ACCharacterBase *Owner;

	ACMultiTool* MultiTool;
	
	/* Can only activate and use this item when in the valid tool mode. */
	bool bValidToolMode;
};
