

#include "CLevelWidget_PStarting.h"
#include "CGameMode.h"
#include "CLevel_PStarting.h"
#include "PlantThatWheat.h"
#include "CEvents_PStarting.h"

#define EObjective ACObjectiveZonePStarting::EObjectiveType

UCLevelWidget_PStarting::UCLevelWidget_PStarting() {
	
	GearTextSuffix = FText::AsNumber(ACEvents_PStarting::NUM_GEARS);
	WheatTextSuffix = FText::AsNumber(ACEvents_PStarting::NUM_WHEAT);
	ObjectiveTextPrefix = FText::FromString("Objective");

	// Set default text w/ suffix/prefix:
	SetGearText(FText::AsNumber(0));
	SetWheatText(FText::AsNumber(0));
}

void UCLevelWidget_PStarting::SetObjectiveText(const FText & NewValue)
{
	ObjectiveText = FText::Format(ObjectiveTextPrefix, NewValue);
}

void UCLevelWidget_PStarting::SetGearText(const FText & NewValue)
{
	GearText = FText::Format(NSLOCTEXT("Widget_PStarting", "Format", "{0}/{1}"), NewValue,GearTextSuffix);
}

void UCLevelWidget_PStarting::SetWheatText(const FText & NewValue)
{
	//WheatText = FText::Format(NewValue, WheatTextSuffix);
	WheatText = FText::Format(NSLOCTEXT("Widget_PStarting", "Format", "{0}/{1}"), NewValue, WheatTextSuffix);
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
		Level->OnCollectWheat.AddUniqueDynamic(this, &UCLevelWidget_PStarting::IncrementWheatCount);
		Level->OnCollectAllWheat.AddUniqueDynamic(this, &UCLevelWidget_PStarting::OnCollectAllWheat);
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

void UCLevelWidget_PStarting::UpdateWheatCount(int32 WheatCount)
{
	if (WheatCount == -1) {
		return;
	}

	this->WheatCount = WheatCount;
	SetWheatText(FText::AsNumber(WheatCount));
}

void UCLevelWidget_PStarting::IncrementGearCount()
{
	SetGearText(FText::AsNumber(++GearCount));
}

void UCLevelWidget_PStarting::IncrementWheatCount()
{
	SetWheatText(FText::AsNumber(++WheatCount));
}

void UCLevelWidget_PStarting::OnCompleteObjective(EPlanet CurPlanet, uint8 OIndex)
{
	if (CurPlanet == EPlanet::P_Starting) { // TODO implement objective
		EObjective Objective = (EObjective) OIndex;
		if (Objective == EObjective::OT_Gears) {
			//SetDisplayText(FText::FromString("You Got all the Gears!"));
			this->OnCollectAllGears();
		}
		else if (Objective == EObjective::OT_Wheat) {
			//SetDisplayText(FText::FromString("You Got all the Wheat!"));
		}
	}
}