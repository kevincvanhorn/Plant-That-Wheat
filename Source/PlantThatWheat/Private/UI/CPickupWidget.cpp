

#include "CPickupWidget.h"
#include "CGameMode.h"
#include "CPlayerState.h"
#include "CLevel_PStarting.h"
#include "CEvents_PStarting.h"

void UCPickupWidget::UpdateWheatCount(int32 WheatCount) {
	if (WheatCount == -1) {
		UE_LOG(LogTemp, Error, TEXT("Invalid PlayerState in CCharacter. "));
		return;
	}

	SetDisplayText(FText::AsNumber(WheatCount));

	this->PlayPickupEffects(); // Start Animation pop-in.

	if (WheatCount == ACEvents_PStarting::NUM_WHEAT && Level) {
		Level->OnCollectAllWheat.Broadcast();
	}
}

void UCPickupWidget::Init(ACLevelScriptActor * Level, ACPlayerState* PlayerState)
{
	if (Level && PlayerState) {
		Level->OnCollectWheat.AddUniqueDynamic(this, &UCPickupWidget::IncrementWheatCount);
		WheatCount = PlayerState->WheatCount; // Initializes from player state.
	}
}

void UCPickupWidget::IncrementWheatCount()
{
	UpdateWheatCount(++WheatCount);
}
