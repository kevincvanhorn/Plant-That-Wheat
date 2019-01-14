

#pragma once

#include "CoreMinimal.h"
#include "CObjectiveZone.h"
#include "CObjectiveZonePStarting.generated.h"

class ACLevel_PStarting;
class UCLevelWidget_PStarting;

UCLASS()
class PLANTTHATWHEAT_API ACObjectiveZonePStarting : public ACObjectiveZone
{
	GENERATED_BODY()

public:
	enum class EObjectiveType : uint8
	{ 
		OT_Gears 	UMETA(DisplayName = "Gears"),
		OT_Wheat 	UMETA(DisplayName = "Wheat"),

		OT_MAX
	};

	struct Objective {
		EObjectiveType Type;
		bool bIsComplete;
	};

	ACObjectiveZonePStarting();

	virtual void BeginPlay() override;

	virtual void OnOverlapMoveable(ACMoveableActor* Moveable) override;
	
	virtual void OnOverlapCharacter(ACCharacterBase* Character) override;

	virtual void OnObjectiveComplete(EPlanet CurPlanet, uint8 Objective) override;

protected:

	void CompleteObjective(Objective& Obj);

private:
	// Collecting Gears Objective:
	int32 NumGearsCollected = 0;

	Objective OB_CollectGears;

	ACLevel_PStarting* Level;
	UCLevelWidget_PStarting *LevelWidget;
};
