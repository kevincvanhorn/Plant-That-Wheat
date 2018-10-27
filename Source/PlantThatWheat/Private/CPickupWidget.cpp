

#include "CPickupWidget.h"
#include "CGameMode.h"
#include "CPlayerState.h"

void UCPickupWidget::UpdateWheatCount(int32 WheatCount) {
	if (WheatCount == -1) {
		UE_LOG(LogTemp, Error, TEXT("Invalid PlayerState in CCharacter. "));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("_VALUE %d"), WheatCount);

	SetDisplayText(FText::AsNumber(WheatCount));
}