

#include "CPickupWidget.h"
#include "CGameMode.h"
#include "CPlayerState.h"

void UCPickupWidget::UpdateWheatCount(int32 WheatCount) {
	if (WheatCount == -1) {
		UE_LOG(LogTemp, Error, TEXT("Invalid PlayerState in CCharacter. "));
		return;
	}

	SetDisplayText(FText::AsNumber(WheatCount));

	this->PlayPickupEffects(); // Start Animation pop-in.
}
