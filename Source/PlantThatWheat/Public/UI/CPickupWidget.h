

#pragma once

#include "CoreMinimal.h"
#include "CTextWidget.h"
#include "CPickupWidget.generated.h"

class ACGameMode;

UCLASS()
class PLANTTHATWHEAT_API UCPickupWidget : public UCTextWidget
{
	GENERATED_BODY()
	
public:
	/** Updates the text display count to @param WheatCount. */
	UFUNCTION()
		void UpdateWheatCount(int32 WheatCount);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Pickup")
		void PlayPickupEffects();

protected:
	ACGameMode* GameMode;
};
