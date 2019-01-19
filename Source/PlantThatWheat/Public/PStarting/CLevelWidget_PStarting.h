

#pragma once

#include "CoreMinimal.h"
#include "CTextWidget.h"
#include "CLevelWidget_PStarting.generated.h"

class ACGameMode;
class ACLevel_PStarting;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
		FText WheatText;

	FText GearTextSuffix;

	FText WheatTextSuffix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
		FText ObjectiveTextPrefix;

	UFUNCTION(BlueprintCallable, Category = "Widgets|Text")
		void SetObjectiveText(const FText& NewValue);

	UFUNCTION(BlueprintCallable, Category = "Widgets|Text")
		void SetGearText(const FText& NewValue);

	UFUNCTION(BlueprintCallable, Category = "Widgets|Text")
		void SetWheatText(const FText& NewValue);

	UFUNCTION()
		void Init(ACGameMode* GameMode, ACLevel_PStarting* Level);


	ACGameMode* GameMode;
	ACLevel_PStarting* Level;

public:
	/** Updates the text display count to @param WheatCount. */
	UFUNCTION()
		void UpdateGearCount(int32 GearCount);

	UFUNCTION()
		void UpdateWheatCount(int32 WheatCount);

	UFUNCTION()
		void IncrementGearCount();

	UFUNCTION()
		void IncrementWheatCount();

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Pickup")
		void OnCollectAllGears();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Pickup")
		void OnCollectAllWheat();

	UFUNCTION()
		void OnCompleteObjective(EPlanet CurPlanet, uint8 Objective);

private:
	int32 GearCount = 0;
	int32 WheatCount = 0;
};
