

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CObjectiveZone.h"
#include "CGameMode.generated.h"

class ACPickup;
class CLevelManagerPStarting;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerCollectWheat); // FOnPlayerCollectWheat, ACPickupActor, PickupType // ONEPARAM
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCollectPickup, class ACPickup*, PickupType);

UCLASS()
class PLANTTHATWHEAT_API ACGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACGameMode();
	
	CLevelManagerPStarting* LevelManager_PStarting;

	void CompleteObjective(EPlanet CurPlanet, uint8 Objective);

	void CompleteWorld(APawn* InstigatorPawn);

	// -------

	UFUNCTION(BlueprintImplementableEvent, Category = "Gamemode")
	void OnWorldCompleted(APawn* InstigatorPawn);

	//UPROPERTY(BlueprintAssignable, Category = "GameMode")
	//FOnPlayerCollectWheat OnPlayerCollectWheat; // Event Delegate when player collects wheat. 

	//UPROPERTY(BlueprintAssignable, Category = "GameMode")
	//	FOnPlayerCollectWheat OnPlayerCollectPickup; // Event Delegate when player collects Pickups. 

public:
	//UPROPERTY(EditDefaultsOnly, Category = "Player")
		//TSubclassOf<UCLevelWidget_PStarting> LevelWidgetClass;
};
