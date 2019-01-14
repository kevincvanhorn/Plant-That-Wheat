

#pragma once

#include "CoreMinimal.h"
#include "CTextWidget.h"
#include "CLevelWidget_PStarting.generated.h"

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API UCLevelWidget_PStarting : public UCTextWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
		FText ObjectiveText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
		FText GearText;

	UFUNCTION(BlueprintCallable, Category = "Widgets|Text")
		void SetObjectiveText(const FText& NewValue);

	UFUNCTION(BlueprintCallable, Category = "Widgets|Text")
		void SetGearText(const FText& NewValue);

public:
	/** Updates the text display count to @param WheatCount. */
	UFUNCTION()
		void UpdateGearCount(int32 GearCount);

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Pickup")
		void OnCollectAllGears();
};
