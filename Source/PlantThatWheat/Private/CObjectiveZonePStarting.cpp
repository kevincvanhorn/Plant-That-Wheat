
#include "CObjectiveZonePStarting.h"
#include "PlantThatWheat.h"
#include "CPlayerState.h"
#include "CLevelManagerPStarting.h"
#include "CLevelWidget_PStarting.h"

ACObjectiveZonePStarting::ACObjectiveZonePStarting() {
	OB_CollectGears = Objective{EObjectiveType::OT_Gears, false };
}

void ACObjectiveZonePStarting::BeginPlay()
{
	/*Level = Cast<ACLevel_PStarting>(GetWorld()->GetLevelScriptActor());
	if (Level) {
		//LevelWidget = Level->LevelWidget;
	}*/
}

void ACObjectiveZonePStarting::OnOverlapMoveable(ACMoveableActor * Moveable)
{
	if (!OB_CollectGears.bIsComplete && Moveable->bCanAffectObjective) {
		NumGearsCollected++;
		Moveable->bCanAffectObjective = false;

		if (LevelWidget) {
			LevelWidget->UpdateGearCount(NumGearsCollected);
		}

		if (NumGearsCollected >= CLevelManagerPStarting::NUM_GEARS) {
			LevelWidget->OnCollectAllGears();

			CompleteObjective(OB_CollectGears);
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
