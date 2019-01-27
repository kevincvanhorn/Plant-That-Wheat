

#include "CLevelScriptActor.h"
#include "CWheatManager.h"


ACLevelScriptActor::ACLevelScriptActor() {

}

void ACLevelScriptActor::BeginPlay() {
	Super::BeginPlay();
	UWorld* const World = GetWorld();

	/*if (World) {
		WheatManager = World->SpawnActor<ACWheatManager>();
	}*/
}