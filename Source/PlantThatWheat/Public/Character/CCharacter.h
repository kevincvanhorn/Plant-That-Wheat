

#pragma once

#include "CoreMinimal.h"
#include "CCharacterBase.h"
#include "CCharacter.generated.h"

class UWidgetComponent;
class UCPickupWidget;
class UCLevelWidget_PStarting;
class UCCompassWidget;
class UCToolWidget;

class ACLevelScriptActor;
//class ACLevel_PStarting;

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

	UCToolWidget* ToolWidget;

	// Blueprint Widget, derriving from PickupWidget.
	UPROPERTY(EditDefaultsOnly, Category = "Player")
		TSubclassOf<UCPickupWidget> PickupWidgetClass;

	UCLevelWidget_PStarting* LevelWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		TSubclassOf<UCLevelWidget_PStarting> LevelWidgetClass;

	UCCompassWidget* CompassWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		TSubclassOf<UCCompassWidget> CompassWidgetClass;

	// Visual indicator of Current tool.
	UPROPERTY(EditDefaultsOnly, Category = "Player")
		UWidgetComponent *ToolWidgetComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		TSubclassOf<UCToolWidget> ToolWidgetClass;

	virtual void SwitchTool() override;

protected:
	virtual void OnPickupItem(ACPickupActor* Pickup) override;
	//ACLevel_PStarting* Level;

	ACLevelScriptActor* Level;
};
