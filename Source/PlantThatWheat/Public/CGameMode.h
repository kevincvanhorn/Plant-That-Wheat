

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerCollectWheat); // FOnPlayerCollectWheat, ACPickupActor, PickupType // ONEPARAM

UCLASS()
class PLANTTHATWHEAT_API ACGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACGameMode();
	
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnPlayerCollectWheat OnPlayerCollectWheat; // Event Delegate when player collects wheat. 
};
