

#pragma once

#include "CoreMinimal.h"
#include "MultiTool/CDefaultTool.h"
#include "CDigTool.generated.h"

class ACPlanetActor;

/**
 * Used for projecting holes using sphere masks instead of shovel tool which should be mowing tool .TODO make this the shovel tool
 */
UCLASS()
class PLANTTHATWHEAT_API ACDigTool : public ACMultiTool
{
	GENERATED_BODY()
	
public:
	ACDigTool();

	virtual void Activate() override;

	virtual void Deactivate() override;

	virtual void Interact() override;

	UFUNCTION(BlueprintImplementableEvent)
		void OnUVTrace(FVector2D UV, FVector HitLoc);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ACPlanetActor* Planet;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		TSubclassOf<UDamageType> DamageType; // Instead of instance. 
};
