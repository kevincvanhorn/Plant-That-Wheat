

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "CDecalActor.generated.h"

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACDecalActor : public ADecalActor
{
	GENERATED_BODY()
	

public:
	ACDecalActor();

	/* Returns true if the animation was successfully started and the driver can move to the next instance
		i.e. the animation is not already in progress. */
	bool UpdateTransform(FVector& NewLoc, FRotator& NewNormal);

	/* Call this from blueprint to communicate w/ watering tool to move to the next decal instance. */
	UFUNCTION(BlueprintCallable)
		void OnFinshAnimation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnStartAnimation();

private:
	bool bIsAnimPlaying;
};
