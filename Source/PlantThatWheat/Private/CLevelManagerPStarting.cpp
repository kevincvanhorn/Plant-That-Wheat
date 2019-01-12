

#include "CLevelManagerPStarting.h"
#include "CGameMode.h"
#include "CObjectiveZonePStarting.h"

CLevelManagerPStarting::CLevelManagerPStarting(ACGameMode* GameMode)
{
	OwningGameMode = GameMode;
}

CLevelManagerPStarting::~CLevelManagerPStarting()
{
}

void CLevelManagerPStarting::CompleteObjective(ACObjectiveZone::EObjectiveType Objective)
{

}
