

#include "CCompassWidget.h"
#include "CGameMode.h"
#include "CCharacter.h"

/*void UCCompassWidget::Init(const ACGameMode* GameMode, const ACCharacter* Character) {
	this->GameMode = GameMode;
	this->Character = Character;
	GetCharacterRotation();
}*/

void UCCompassWidget::Init(ACGameMode* GameMode, ACCharacter* Character) {
	this->GameMode = GameMode;
	this->Character = Character;
}

FVector UCCompassWidget::GetCharacterRotation()
{

	if (Character) {
		UE_LOG(LogTemp, Warning, TEXT("YES---------------YES %s"), *Character->GetCurrentForwardDirection().ToString());

		return Character->GetCurrentForwardDirection();
	}
	return FVector::ZeroVector;
}
