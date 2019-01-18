

#pragma once

#include "CoreMinimal.h"
#include "UI/CTextWidget.h"
#include "CCompassWidget.generated.h"

class ACGameMode;
class ACCharacter;

UCLASS()
class PLANTTHATWHEAT_API UCCompassWidget : public UCTextWidget
{
	GENERATED_BODY()
	
public:
	//UFUNCTION()
	//	void Init(const ACGameMode* GameMode, const ACCharacter* Character);

	void Init(ACGameMode* GameMode, ACCharacter* const Character);

	UFUNCTION(BlueprintCallable, Category = "Widgets|Compass")
		float GetRotationOffset();

protected:
	ACGameMode* GameMode;
	ACCharacter* Character;

private:

	//const ACGameMode* GameMode;
	//const ACCharacter* Character;
};
