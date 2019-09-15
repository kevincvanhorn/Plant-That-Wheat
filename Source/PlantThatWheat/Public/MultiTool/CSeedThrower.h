

#pragma once

#include "CoreMinimal.h"
#include "MultiTool/CMultiTool.h"
#include "CWheatSpawnable.h"
#include "CSeedThrower.generated.h"

class ACWheatManager;
class ACWheatSpawnable;

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

	/** Seedling class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<ACWheatSpawnable> SeedlingClass;

	UPROPERTY(EditDefaultsOnly, Category = Foliage)
		UStaticMesh* WheatMesh_Dead;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		TSubclassOf<UDamageType> DamageType; // Instead of instance. 

	ACWheatManager* WheatManager;
};
