#pragma once

#include "CoreMinimal.h"
#include "CUsableActor.h"
#include "CustomPhysicsActor.h"
#include "CMoveableActor.generated.h"

class ACCharacterBase;

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
	
protected:
	bool bIsBeingHeld;

	void PickUp();

	void SetDown();

	FVector ForwardVector;

	FVector HoldLocation;

	ACCharacterBase * Owner;
};
