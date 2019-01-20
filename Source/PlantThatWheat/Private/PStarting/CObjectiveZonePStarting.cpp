
#include "CObjectiveZonePStarting.h"
#include "PlantThatWheat.h"
#include "CPlayerState.h"
//#include "CLevelManagerPStarting.h"
#include "CEvents_PStarting.h"
#include "CLevelWidget_PStarting.h"

#include "CMoveableActor.h"

ACObjectiveZonePStarting::ACObjectiveZonePStarting() {
	OB_CollectGears = Objective{EObjectiveType::OT_Gears, false };
}

void ACObjectiveZonePStarting::BeginPlay()
{
	Level = Cast<ACLevel_PStarting>(GetWorld()->GetLevelScriptActor());
}

void ACObjectiveZonePStarting::OnOverlapMoveable(ACMoveableActor * Moveable)
{
	if (!OB_CollectGears.bIsComplete && Moveable->bCanAffectObjective) {
		NumGearsCollected++;
		Moveable->bCanAffectObjective = false;

		/*if (LevelWidget) {
			LevelWidget->UpdateGearCount(NumGearsCollected);
		}*/
		// Call binded event here.
		/*if (LevelWidget) {
			LevelWidget->UpdateGearCount(NumGearsCollected);
		}*/
		if (Level) {
			Level->OnCollectGear.Broadcast();
		}
		if (NumGearsCollected >= ACEvents_PStarting::NUM_GEARS) {
			//LevelWidget->OnCollectAllGears();
			CompleteObjective(OB_CollectGears);
		}
		if (Moveable) {
			Moveable->OnObjectiveOverlap();
		}
	}
}

void ACObjectiveZonePStarting::OnOverlapCharacter(ACCharacterBase * Character)
{

}

void ACObjectiveZonePStarting::OnObjectiveComplete(EPlanet CurPlanet, uint8 Objective)
{
	// Tell the GameMode that an objective has been completed
	Super::OnObjectiveComplete(CurPlanet, Objective);
}

void ACObjectiveZonePStarting::CompleteObjective(Objective& Obj)
{
	Obj.bIsComplete = true;
	OnObjectiveComplete(EPlanet::P_Starting, (uint8)(Obj.Type));
}
