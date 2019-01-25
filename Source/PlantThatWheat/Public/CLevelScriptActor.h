

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CLevelScriptActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollectWheat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollectAllWheat);


class ADirectionalLight;

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Objectives")
		FOnCollectWheat OnCollectWheat;

	UPROPERTY(BlueprintAssignable, Category = "Objectives")
		FOnCollectAllWheat OnCollectAllWheat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Lighting)
	ADirectionalLight * Sun;
};
