

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameMode.generated.h"

class ACPickup;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerCollectWheat); // FOnPlayerCollectWheat, ACPickupActor, PickupType // ONEPARAM
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCollectPickup, class ACPickup*, PickupType);

UCLASS()
class PLANTTHATWHEAT_API ACGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACGameMode();
	
	//UPROPERTY(BlueprintAssignable, Category = "GameMode")
	//FOnPlayerCollectWheat OnPlayerCollectWheat; // Event Delegate when player collects wheat. 

	//UPROPERTY(BlueprintAssignable, Category = "GameMode")
	//	FOnPlayerCollectWheat OnPlayerCollectPickup; // Event Delegate when player collects Pickups. 
};
