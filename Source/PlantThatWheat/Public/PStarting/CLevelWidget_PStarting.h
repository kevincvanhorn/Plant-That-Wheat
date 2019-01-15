

#pragma once

#include "CoreMinimal.h"
#include "CTextWidget.h"
#include "CLevelWidget_PStarting.generated.h"

class ACGameMode;
class ACLevel_PStarting;

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API UCLevelWidget_PStarting : public UCTextWidget
{
	GENERATED_BODY()
	
public:
	UCLevelWidget_PStarting();

	//virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
		FText ObjectiveText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
		FText GearText;

	UFUNCTION(BlueprintCallable, Category = "Widgets|Text")
		void SetObjectiveText(const FText& NewValue);

	UFUNCTION(BlueprintCallable, Category = "Widgets|Text")
		void SetGearText(const FText& NewValue);

	UFUNCTION()
		void Init(ACGameMode* GameMode, ACLevel_PStarting* Level);

	ACGameMode* GameMode;
	ACLevel_PStarting* Level;

public:
	/** Updates the text display count to @param WheatCount. */
	UFUNCTION()
		void UpdateGearCount(int32 GearCount);

	UFUNCTION()
		void IncrementGearCount();

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Pickup")
		void OnCollectAllGears();

	UFUNCTION()
		void OnCompleteObjective(EPlanet CurPlanet, uint8 Objective);

private:
	int32 GearCount = 0;
};
