

#include "CGameMode.h"
#include "CPlayerState.h"
//#include "CLevelManagerPStarting.h"
#include "CEvents_PStarting.h"
#include "CObjectiveZone.h"

#include "CPlayerController.h"
#include "CCharacter.h"
#include "CPlayerState.h"

ACGameMode::ACGameMode() {
	PlayerStateClass = ACPlayerState::StaticClass();  // Sets CPlayerState as default Player State.

	PlayerControllerClass = ACPlayerController::StaticClass();
	DefaultPawnClass = ACCharacter::StaticClass();
	GameStateClass = ACPlayerState::StaticClass();


	//LevelManager_PStarting = new CLevelManagerPStarting(this);
	//Manager_PStarting = NewObject<UCLevelManagerPStarting>();
	//Manager_PStarting->SetGameMode(this);
}

/*void ACGameMode::CompleteObjective(EPlanet CurPlanet, uint8 Objective)
{
	if (CurPlanet == EPlanet::P_Starting && LevelManager_PStarting) {
		LevelManager_PStarting->CompleteObjective(Objective);
	}
}*/

void ACGameMode::BeginPlay()
{
	Events_PStarting = GetWorld()->SpawnActor<ACEvents_PStarting>();
	if (Events_PStarting) {
		Events_PStarting->SetGameMode(this);
	}
}

void ACGameMode::CompleteWorld(APawn * InstigatorPawn)
{
	if (InstigatorPawn) {
		InstigatorPawn->DisableInput(nullptr);
	}

	this->OnWorldCompleted(InstigatorPawn);
}
