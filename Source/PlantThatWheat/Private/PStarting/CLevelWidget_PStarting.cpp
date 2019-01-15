

#include "CLevelWidget_PStarting.h"
#include "CGameMode.h"
#include "CLevel_PStarting.h"
#include "PlantThatWheat.h"

UCLevelWidget_PStarting::UCLevelWidget_PStarting() {
	
	//this->RemoveFromViewport();
	/*
	ACGameMode* GM = Cast<ACGameMode>(GetWorld()->GetAuthGameMode());

	if (GM) {
		// Bind to Delegate.
		GM->OnCompleteObjective.AddUniqueDynamic(this, &UCLevelWidget_PStarting::OnCompleteObjective);
	}
	*/
}

void UCLevelWidget_PStarting::SetObjectiveText(const FText & NewValue)
{
	ObjectiveText = NewValue;
}

void UCLevelWidget_PStarting::SetGearText(const FText & NewValue)
{
	GearText = NewValue;
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
		SetObjectiveText(FText::FromString("You Got all the Gears!"));
	}
}
