/* Debug tool to test linetraces for hitscan. */

#pragma once

#include "CoreMinimal.h"
#include "CMultiTool.h"
#include "CGunTool.generated.h"

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACGunTool : public ACMultiTool
{
	GENERATED_BODY()
	
public:
	ACGunTool();

	virtual void Activate() override;

	virtual void Deactivate() override;

	virtual void Interact() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		TSubclassOf<UDamageType> DamageType; // Instead of instance. 

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		UParticleSystem * MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		UParticleSystem * DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		UParticleSystem * FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
		UParticleSystem * TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "MultiTool")
		TSubclassOf<UCameraShake> FireCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "MultiTool")
		float BaseDamage;
	
	virtual void TraceFireEffects(FVector TraceEnd) override;
	virtual void TraceHitEffects(FHitResult const& HitInfo) override;
	
	virtual void ApplyDamage(AActor * DamagedActor, FVector const & HitFromDirection, FHitResult const & HitInfo, AController * EventInstigator) override;
	
};
