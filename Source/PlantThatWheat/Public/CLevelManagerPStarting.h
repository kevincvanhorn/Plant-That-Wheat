

#pragma once

#include "CoreMinimal.h"

class ACGameMode;

/**
 * 
 */

class PLANTTHATWHEAT_API CLevelManagerPStarting
{
public:
	CLevelManagerPStarting(ACGameMode* GameMode);
	~CLevelManagerPStarting();

	void CompleteObjective(ACObjectiveZone::EObjectiveType Objective);

	ACGameMode* OwningGameMode;
};