

#pragma once

#include "CoreMinimal.h"
#include "CTextWidget.h"
#include "CPickupWidget.generated.h"

class ACGameMode;
class ACLevel_PStarting;
class ACPlayerState;

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

	UFUNCTION()
		void Init(ACLevel_PStarting* Level, ACPlayerState* PlayerState);

	UFUNCTION()
		void IncrementWheatCount();

protected:
	ACGameMode* GameMode;

	ACLevel_PStarting * Level;

	int32 WheatCount = 0;
};
