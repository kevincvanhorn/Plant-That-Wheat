#pragma once

#include "CoreMinimal.h"
#include "CUsableActor.h"
#include "CustomPhysicsActor.h"
#include "CMoveableActor.generated.h"

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACMoveableActor : public ACustomPhysicsActor
{
	GENERATED_BODY()

public:
	ACMoveableActor();

	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(EditAnywhere, Category = "Moveable Actor")
	//UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Moveable Actor")
	float PawnOffset;

	virtual bool OnUsed_Implementation(ACMultiTool * Tool) override;

	virtual bool StartFocus_Implementation() override;

	virtual bool EndFocus_Implementation() override;
	
protected:
	bool bIsBeingHeld;

	void PickUp();

	void SetDown();

	FVector ForwardVector;

	FVector HoldLocation;

	APawn * Owner;
};
