

#include "CGameMode.h"
#include "CPlayerState.h"

ACGameMode::ACGameMode() {
	PlayerStateClass = ACPlayerState::StaticClass();  // Sets CPlayerState as default Player State.
}