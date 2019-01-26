

#pragma once

#include "CoreMinimal.h"
//#include "Animation/SkeletalMeshActor.h"
#include "GameFramework/Actor.h"
#include "CWheatSpawnable.generated.h"

UCLASS()
class PLANTTHATWHEAT_API ACWheatSpawnable : public AActor
{
	GENERATED_BODY()

public:
	ACWheatSpawnable();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		bool bIsHealthy;

	UFUNCTION(BlueprintImplementableEvent)
		void OnSpawnHealthy();

	UFUNCTION(BlueprintImplementableEvent)
		void OnSpawnUnhealthy();

	void SpawnHealthy();
	void SpawnUnhealthy();

private:
	void Init(bool bIsHealthy);
};
