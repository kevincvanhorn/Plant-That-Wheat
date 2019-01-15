

#include "CLevelWidget_PStarting.h"
#include "CGameMode.h"
#include "CLevel_PStarting.h"
#include "PlantThatWheat.h"
#include "CEvents_PStarting.h"

UCLevelWidget_PStarting::UCLevelWidget_PStarting() {
	
	GearTextSuffix = FText::AsNumber(ACEvents_PStarting::NUM_GEARS);
	ObjectiveTextPrefix = FText::FromString("Objective");
}

void UCLevelWidget_PStarting::SetObjectiveText(const FText & NewValue)
{
	ObjectiveText = FText::Format(ObjectiveTextPrefix, NewValue);
}

void UCLevelWidget_PStarting::SetGearText(const FText & NewValue)
{
	GearText = FText::Format(NewValue,GearTextSuffix);
}

void UCLevelWidget_PStarting::Init(ACGameMode * GameMode, ACLevel_PStarting* Level)
{
	this->GameMode = GameMode;
	this->Level = Level;
	if (GameMode) {
		// Bind to Delegate.
		GameMode->OnCompleteObjective.AddUniqueDynamic(this, &UCLevelWidget_PStarting::OnCompleteObjective);
	}
	if (Level) {
		Level->OnCollectGear.AddUniqueDynamic(this, &UCLevelWidget_PStarting::IncrementGearCount);
	}
}

void UCLevelWidget_PStarting::UpdateGearCount(int32 GearCount)
{
	if (GearCount == -1) {
		return;
	}

	this->GearCount = GearCount;
	SetGearText(FText::AsNumber(GearCount));
}

void UCLevelWidget_PStarting::IncrementGearCount()
{
	SetGearText(FText::AsNumber(++GearCount));
}

void UCLevelWidget_PStarting::OnCompleteObjective(EPlanet CurPlanet, uint8 Objective)
{
	if (CurPlanet == EPlanet::P_Starting) { // TODO implement objective
		SetDisplayText(FText::FromString("You Got all the Gears!"));
	}
}