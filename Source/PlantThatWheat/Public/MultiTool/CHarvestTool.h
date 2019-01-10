#pragma once

#include "CoreMinimal.h"
#include "CMultiTool.h"
#include "CHarvestTool.generated.h"

class UCStaticFoliageComponent;

UCLASS()
class PLANTTHATWHEAT_API ACHarvestTool : public ACMultiTool
{
	GENERATED_BODY()

public:
	ACHarvestTool();

	virtual void Activate() override;

	virtual void Deactivate() override;

	virtual void BeginPlay() override;

	virtual void Interact() override;

	/** The x (Right) y (Forward) z (Up) offset from the transform of the Player. */
	UPROPERTY(EditAnywhere, Category = "Moveable Actor")
		FVector PawnOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		FVector ColliderOffset;

	UFUNCTION()
		void OnHarvestBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	TArray<UCStaticFoliageComponent*> FoliageArray;

	bool bIsActive = false;
};