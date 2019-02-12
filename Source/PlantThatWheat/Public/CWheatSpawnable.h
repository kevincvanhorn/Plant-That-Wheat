

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
		UStaticMeshComponent* SeedlingMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* WheatMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		bool bIsHealthy;

	UFUNCTION(BlueprintImplementableEvent)
		void OnSpawnHealthy();

	UFUNCTION(BlueprintImplementableEvent)
		void OnSpawnUnhealthy();

	UFUNCTION(BlueprintCallable)
		void ConvertToInstance();

	UFUNCTION(BlueprintCallable)
		void SetReadyToDestroy(bool bIsReady);

	UFUNCTION()
		void OnBeginWheatOverlap(AActor* OverlappedActor, AActor* OtherActor);

	//UFUNCTION(BlueprintImplementableEvent)
	//	void OnDestroy();

	void SpawnHealthy();
	void SpawnUnhealthy();

protected:
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	bool bReadyToDestroy = false;

/* Watering Tool: */
public:
	void OnBeginWatering();
	void OnEndWatering();

	bool bIsFullyGrown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeToWater;

protected:
	/* Handle for watering timer. */
	FTimerHandle WateringHandle;

	UFUNCTION()
		void OnFullyWatered();

	UFUNCTION(BlueprintImplementableEvent)
		void OnFullyWateredBP();

	float TimeWatered;
};
