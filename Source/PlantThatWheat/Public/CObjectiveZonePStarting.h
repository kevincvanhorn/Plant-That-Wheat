

#pragma once

#include "CoreMinimal.h"
#include "CObjectiveZone.h"
#include "CObjectiveZonePStarting.generated.h"

UCLASS()
class PLANTTHATWHEAT_API ACObjectiveZonePStarting : public ACObjectiveZone
{
	GENERATED_BODY()

public:
	enum class EObjectiveType : uint8
	{ 
		OT_Gears 	UMETA(DisplayName = "Gears"),
		OT_Wheat 	UMETA(DisplayName = "Wheat")
	};

public:
	virtual void OnOverlapMoveable(ACMoveableActor* Moveable) override;
	
	virtual void OnOverlapCharacter(ACCharacterBase* Character) override;

	//virtual void OnObjectiveComplete(EObjectiveType Objective) override;
};
