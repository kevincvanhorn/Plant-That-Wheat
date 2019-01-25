

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CLevelScriptActor.h"
#include "CLevel_PStarting.generated.h"

/*
* Delegates serve as an intermediary for 2+ classes.
* An event is Brodcast to this Level class and then relayed to classes like the UI.
* As such, classes only need to know the Level Script Actor, which can be retrieved in any other Actor derived class easily.
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollectGear);

class UCLevelWidget_PStarting;
class UWidgetComponent;
class ACCharacter;

UCLASS()
class PLANTTHATWHEAT_API ACLevel_PStarting : public ACLevelScriptActor
{
	GENERATED_BODY()
	
	
public:
	ACLevel_PStarting();


	ACCharacter* Character;

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "Objectives")
		FOnCollectGear OnCollectGear;

protected:
	// Indicate Objectives & Update persistent
	//UPROPERTY(EditDefaultsOnly, Category = "Player")
	//	UWidgetComponent *LevelWidgetComp;
};
