/** The hub for tools that can be switched out - defining modes of the player's interactions with the environment. 
*	[18.09.14] - Currently combination of all the multitools before switching is enabled
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMultiTool.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;
class UInstancedStaticMeshComponent;
//class UStaticMesh;

UCLASS()
class PLANTTHATWHEAT_API ACMultiTool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACMultiTool();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	void PlayFireEffects(FVector TraceEnd);

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

	void PlantOnHit(FVector TraceEnd);

	UPROPERTY(EditDefaultsOnly, Category = "MultiTool")
	UStaticMesh *PlantAsset;

	UInstancedStaticMeshComponent* foliageMeshComponent;

public:	
	UFUNCTION(BlueprintCallable, Category = "MultiTool")
	virtual void Fire();


	
};
