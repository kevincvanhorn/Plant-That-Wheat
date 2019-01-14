

#include "CLevelManagerPStarting.h"
#include "CGameMode.h"
#include "CObjectiveZonePStarting.h" 
#include "CLevelWidget_PStarting.h"
#include "CLevel_PStarting.h"

CLevelManagerPStarting::CLevelManagerPStarting(ACGameMode* GameMode)
{
	/*Level = Cast<ACLevel_PStarting>(GameMode->GetWorld()->GetLevelScriptActor());
	if (Level) {
		LevelWidget = Level->LevelWidget;
	}*/
}

CLevelManagerPStarting::~CLevelManagerPStarting()
{

}

void CLevelManagerPStarting::CompleteObjective(uint8 OIndex)
{
	// Convert to enum:
	if (OIndex >= 0 && OIndex < (uint8)(EObjective::OT_MAX)) {
		EObjective Objective = (EObjective)(OIndex);
		if (Objective == EObjective::OT_Gears) {
			UE_LOG(LogTemp, Warning, TEXT("Gear Mission Complete! "));
			CompleteGearObjective();
		}
		else if (Objective == EObjective::OT_Wheat) {

		}
	}
}

void CLevelManagerPStarting::CompleteGearObjective()
{
	/*if (LevelWidget) {
		LevelWidget->OnCollectAllGears();
	}*/
}



