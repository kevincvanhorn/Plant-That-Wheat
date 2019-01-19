

#include "CPlayerState.h"
#include "CGameMode.h"
#include "CLevel_PStarting.h"

void ACPlayerState::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("PLAYER STATE INIT"));

	Level = Cast<ACLevel_PStarting>(GetWorld()->GetLevelScriptActor()); // TODO: Make this generic for all levels

	if (Level) {
		Level->OnCollectWheat.AddUniqueDynamic(this, &ACPlayerState::IncrementWheatCount);
	}
}

void ACPlayerState::IncrementWheatCount()
{
	WheatCount++;
}