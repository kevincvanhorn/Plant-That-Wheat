

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CObjectiveZone.h"
#include "CGameMode.generated.h"

class ACPickup;
//class UCLevelManagerPStarting;
class ACEvents_PStarting;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerCollectWheat); // FOnPlayerCollectWheat, ACPickupActor, PickupType // ONEPARAM
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCollectPickup, class ACPickup*, PickupType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCompleteObjective, EPlanet, CurPlanet, uint8, Objective);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleteObjective_PStarting, uint8, Objective);

UCLASS()
class PLANTTHATWHEAT_API ACGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACGameMode();

	virtual void BeginPlay() override;
	
	//UCLevelManagerPStarting* Manager_PStarting;

	ACEvents_PStarting* Events_PStarting;

	//void CompleteObjective(EPlanet CurPlanet, uint8 Objective); // TODO: replace this with the delegate.

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnCompleteObjective OnCompleteObjective;

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnCompleteObjective_PStarting OnCompleteObjective_PStarting;

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
