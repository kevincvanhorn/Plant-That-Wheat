

#pragma once

#include "CoreMinimal.h"
#include "MultiTool/CDefaultTool.h"
#include "CDigTool.generated.h"

class ACPlanetActor;
class ACLevelScriptActor;

/**
 * Used for projecting holes using sphere masks instead of shovel tool which should be mowing tool .TODO make this the shovel tool
 */
UCLASS()
class PLANTTHATWHEAT_API ACDigTool : public ACMultiTool
{
	GENERATED_BODY()
	
public:
	ACDigTool();

	void Init(ACLevelScriptActor * Level);

	UFUNCTION(BlueprintImplementableEvent)
		void OnInit();

	virtual void Activate() override;

	virtual void Deactivate() override;

	virtual void Interact() override;

	UFUNCTION(BlueprintImplementableEvent)
		void OnUVTrace(FVector2D UV, FVector HitLoc);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ACPlanetActor* Planet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HoleRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HoleHardness;

protected:
	virtual void BeginPlay() override;

	ACLevelScriptActor * Level;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		TSubclassOf<UDamageType> DamageType; // Instead of instance. 

	void AddDigArea(FVector & DigCentroid);

	/* Radius with harness falloff applied. */
	float AdjustedHoleRadius;
};
