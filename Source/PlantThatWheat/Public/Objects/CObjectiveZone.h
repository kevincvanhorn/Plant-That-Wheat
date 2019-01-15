

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantThatWheat.h"
#include "CObjectiveZone.generated.h"

class UBoxComponent;
class ACMoveableActor;
class ACCharacterBase;

UCLASS()
class PLANTTHATWHEAT_API ACObjectiveZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACObjectiveZone();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* OverlapComp;

public:
	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	virtual void OnOverlapMoveable(ACMoveableActor* Moveable);

	virtual void OnOverlapCharacter(ACCharacterBase* Character);

	virtual void OnObjectiveComplete(EPlanet CurPlanet, uint8 Objective);
};
