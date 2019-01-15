

#pragma once

#include "CoreMinimal.h"
#include "CCharacterBase.h"
#include "CCharacter.generated.h"

class UWidgetComponent;
class UCPickupWidget;
class UCLevelWidget_PStarting;
class UCCompassWidget;

UCLASS()
class PLANTTHATWHEAT_API ACCharacter : public ACCharacterBase
{
	GENERATED_BODY()
	
public:
	ACCharacter();

	virtual void BeginPlay() override;

	// Visual indicator of Pickups (Icon & Text).
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	UWidgetComponent *PickupWidgetComp;

	// Blueprint Widget, derriving from PickupWidget.
	UPROPERTY(EditDefaultsOnly, Category = "Player")
		TSubclassOf<UCPickupWidget> PickupWidgetClass;

	UFUNCTION()
	void UpdatePickupDisplay();

	UCLevelWidget_PStarting* LevelWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		TSubclassOf<UCLevelWidget_PStarting> LevelWidgetClass;

	UCCompassWidget* CompassWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		TSubclassOf<UCCompassWidget> CompassWidgetClass;

protected:
	virtual void OnPickupItem(ACPickupActor* Pickup) override;

};
