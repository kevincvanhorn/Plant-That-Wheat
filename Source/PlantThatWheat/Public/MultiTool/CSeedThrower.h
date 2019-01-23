

#pragma once

#include "CoreMinimal.h"
#include "MultiTool/CMultiTool.h"
#include "CSeedThrower.generated.h"

/**s
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACSeedThrower : public ACMultiTool
{
	GENERATED_BODY()

public:
	ACSeedThrower();
	
	virtual void Activate() override;

	virtual void Deactivate() override;

	virtual void Interact() override;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ACProjectileActor> ProjectileClass;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		TSubclassOf<UDamageType> DamageType; // Instead of instance. 

};
