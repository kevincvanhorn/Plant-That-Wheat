

#include "CPlayerState.h"
#include "CGameMode.h"

void ACPlayerState::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("PLAYER STATE INIT"));
}

void ACPlayerState::IncrementWheatCount()
{
	WheatCount++;
}