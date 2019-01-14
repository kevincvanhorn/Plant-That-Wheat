

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	/** Player's total units of wheat collected. */
	UPROPERTY(BlueprintReadOnly, Category = PlayerState)
	int32 WheatCount;

	/** Player's total Gears collected. */
	UPROPERTY(BlueprintReadOnly, Category = PlayerState)
		int32 GearCount;

	virtual void BeginPlay() override;

	UFUNCTION()
		void IncrementWheatCount();

	UFUNCTION()
		const int32 GetWheatCount() { return WheatCount; }
};
