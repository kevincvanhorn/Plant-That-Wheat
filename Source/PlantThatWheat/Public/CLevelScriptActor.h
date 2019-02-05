

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CLevelScriptActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollectWheat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollectAllWheat);


class ADirectionalLight;
class ACWheatManager;
class ACPlanetActor;

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ACLevelScriptActor();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "Objectives")
		FOnCollectWheat OnCollectWheat;

	UPROPERTY(BlueprintAssignable, Category = "Objectives")
		FOnCollectAllWheat OnCollectAllWheat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Lighting)
	ADirectionalLight * Sun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Planet)
		ACPlanetActor* Planet;
	//ACWheatManager* WheatManager;
};
