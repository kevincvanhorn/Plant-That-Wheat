

#pragma once

#include "CoreMinimal.h"

#define EObjective ACObjectiveZonePStarting::EObjectiveType

class ACGameMode;
class ACObjectiveZonePStarting;
class UCLevelWidget_PStarting;
class ACLevel_PStarting;

/**
 * Serves as an Extension of GameMode to organize events into each planet.
 */

class PLANTTHATWHEAT_API CLevelManagerPStarting
{
public:
	CLevelManagerPStarting(ACGameMode* GameMode);
	~CLevelManagerPStarting();

	static const int32 NUM_GEARS = 4;

	/* Called when an objective on the starting planet is completed. */
	void CompleteObjective(uint8 OIndex);

	ACGameMode* OwningGameMode;

	void CompleteGearObjective();


private:
	//ACLevel_PStarting* Level;
	//UCLevelWidget_PStarting *LevelWidget;
};