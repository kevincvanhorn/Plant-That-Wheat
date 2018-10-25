

#pragma once

#include "CoreMinimal.h"
#include "CUsableActor.h"
#include "CustomPhysicsActor.h"
#include "CPickupActor.generated.h"

class ACCharacterBase;
class USphereComponent;

UCLASS()
class PLANTTHATWHEAT_API ACPickupActor : public ACustomPhysicsActor
{
	GENERATED_BODY()
	
public:
	ACPickupActor();

	/** Pick up item via Player interact. */
	virtual bool OnUsed_Implementation(ACMultiTool * Tool) override;

	virtual bool StartFocus_Implementation() override;

	virtual bool EndFocus_Implementation() override;
	
	ACCharacterBase * Owner;

	/** Can the actor be picked up? */
	bool bIsActive;

	/** Should the Player pick up the object on overlap? If false: must be clicked. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup Actor")
	bool bUseOnOverlap;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup Actor")
	USphereComponent* OverlapComp;

	/** Pick up item via Player overlap. */
	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;

};
