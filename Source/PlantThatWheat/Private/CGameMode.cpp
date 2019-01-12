

#include "CGameMode.h"
#include "CPlayerState.h"
#include "CLevelManagerPStarting.h"
#include "CObjectiveZone.h"


ACGameMode::ACGameMode() {
	PlayerStateClass = ACPlayerState::StaticClass();  // Sets CPlayerState as default Player State.

	LevelManager_PStarting = new CLevelManagerPStarting(this);
}

void ACGameMode::CompleteObjective(EPlanet CurPlanet, ACObjectiveZone::EObjectiveType Objective)
{
	if (CurPlanet == EPlanet::P_Starting && LevelManager_PStarting) {
		LevelManager_PStarting->CompleteObjective(Objective);
	}
}

void ACGameMode::CompleteWorld(APawn * InstigatorPawn)
{
	if (InstigatorPawn) {
		InstigatorPawn->DisableInput(nullptr);
	}

	this->OnWorldCompleted(InstigatorPawn);
}
