

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CLevel_PStarting.generated.h"

class UCLevelWidget_PStarting;
class UWidgetComponent;
class ACCharacter;

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACLevel_PStarting : public ALevelScriptActor
{
	GENERATED_BODY()
	
	
public:
	ACLevel_PStarting();


	ACCharacter* Character;

	virtual void BeginPlay() override;

protected:
	// Indicate Objectives & Update persistent
	//UPROPERTY(EditDefaultsOnly, Category = "Player")
	//	UWidgetComponent *LevelWidgetComp;
};
