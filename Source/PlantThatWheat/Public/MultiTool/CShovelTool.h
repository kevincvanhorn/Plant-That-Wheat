#pragma once

#include "CoreMinimal.h"
#include "CMultiTool.h"
#include "CShovelTool.generated.h"

/**
 * 
 */
class UMaterial;
class AStaticMeshActor;
class UBoxComponent;
class UCStaticFoliageComponent;

UCLASS()
class PLANTTHATWHEAT_API ACShovelTool : public ACMultiTool
{
	GENERATED_BODY()

public:
	ACShovelTool();

	virtual void Tick(float DeltaSeconds) override;

	virtual void Activate() override;

	virtual void Deactivate() override;

	virtual void BeginPlay() override;

	/** The x (Right) y (Forward) z (Up) offset from the transform of the Player. */
	UPROPERTY(EditAnywhere, Category = "Moveable Actor")
		FVector PawnOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* GroundCollider;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* BoxCollider;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		FVector ColliderOffset;

	UFUNCTION()
	void OnBeginShovelOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnEndShovelOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void UpdateDesiredLocation(float DeltaTime);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		float CameraRotationLagSpeed = 10.f;

private:
	//TArray<UInstancedStaticMeshComponent*> FoliageArray;
	TArray<UCStaticFoliageComponent*> FoliageArray;
	void RemoveFoliageOnOverlap();

	bool bIsActive = false;

	float PreviousYaw;
};