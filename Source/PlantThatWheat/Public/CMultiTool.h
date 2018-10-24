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

	bool bCanSingleTrace;

	float SingleTraceDist;

	virtual void TraceFireEffects(FVector TraceEnd);
	virtual void TraceHitEffects(FHitResult const& HitInfo);

	bool bCanDamage; // Enable Damage in trace if active.
	virtual void ApplyDamage(AActor* DamagedActor, FVector const& HitFromDirection, FHitResult const& HitInfo, AController* EventInstigator);

public:	
	UFUNCTION(BlueprintCallable, Category = "MultiTool")
	virtual void DoSingleTrace();

	/** Behaviour on primary button press. */
	virtual void Interact();

	APawn * MyOwner;
	APlayerController* OwnerController;

	virtual void Activate();

	virtual void Deactivate();

};
