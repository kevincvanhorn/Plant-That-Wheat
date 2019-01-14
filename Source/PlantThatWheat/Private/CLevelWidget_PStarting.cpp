

#include "CLevelWidget_PStarting.h"

void UCLevelWidget_PStarting::SetObjectiveText(const FText & NewValue)
{
	ObjectiveText = NewValue;
}

void UCLevelWidget_PStarting::SetGearText(const FText & NewValue)
{
	GearText = NewValue;
}

void UCLevelWidget_PStarting::UpdateGearCount(int32 GearCount)
{
	if (GearCount == -1) {
		return;
	}

	SetGearText(FText::AsNumber(GearCount));
}
