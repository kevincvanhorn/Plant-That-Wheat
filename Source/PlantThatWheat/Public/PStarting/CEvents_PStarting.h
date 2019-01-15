#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantThatWheat.h"
#include "CObjectiveZonePStarting.h"

#include "CEvents_PStarting.generated.h"

#define EObjective ACObjectiveZonePStarting::EObjectiveType

class ACGameMode;

UCLASS()
class PLANTTHATWHEAT_API ACEvents_PStarting : public AActor
{
	GENERATED_BODY()
	
public:	
	/*struct Event {
		FText EventText;
	};*/

	//TQueue<Event> Events;

	// Sets default values for this actor's properties
	ACEvents_PStarting();

	static const int32 NUM_GEARS = 2;

	/* Called when an objective on the starting planet is completed. */
	void CompleteObjective(uint8 OIndex);

	UFUNCTION()
		void OnCompleteObjective(EPlanet CurPlanet, uint8 Objective);

	ACGameMode* OwningGameMode;

	void CompleteGearObjective();

	void SetGameMode(ACGameMode* const GameMode);

private:
	ACGameMode* GameMode;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
