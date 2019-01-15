

#include "CEvents_PStarting.h"
#include "CGameMode.h"


// Sets default values
ACEvents_PStarting::ACEvents_PStarting()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACEvents_PStarting::BeginPlay()
{
	Super::BeginPlay();

	if (GameMode) {
		// Bind to Delegate.
		GameMode->OnCompleteObjective.AddUniqueDynamic(this, &ACEvents_PStarting::OnCompleteObjective);
	}
}

void ACEvents_PStarting::CompleteObjective(uint8 OIndex)
{
	// Convert to enum:
	if (OIndex >= 0 && OIndex < (uint8)(EObjective::OT_MAX)) {
		EObjective Objective = (EObjective)(OIndex);
		if (Objective == EObjective::OT_Gears) {
			UE_LOG(LogTemp, Warning, TEXT("Gear Mission Complete! "));
			CompleteGearObjective();
		}
		else if (Objective == EObjective::OT_Wheat) {

		}
	}
}

void ACEvents_PStarting::OnCompleteObjective(EPlanet CurPlanet, uint8 Objective)
{
	if (CurPlanet == EPlanet::P_Starting) {
		CompleteObjective(Objective);
	}
}

void ACEvents_PStarting::CompleteGearObjective()
{
	/*if (LevelWidget) {
		LevelWidget->OnCollectAllGears();
	}*/
}

void ACEvents_PStarting::SetGameMode(ACGameMode * const GameMode)
{
	this->GameMode = GameMode;
}



// Called every frame
void ACEvents_PStarting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

