

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

bool ACLevelScriptActor::bWithinDigArea(FVector& QueryLoc)
{
	for (FSphere* Elem : PlanetHoles) {
		if (Elem->IsInside(QueryLoc)) return true;
	}
	return false;
}

void ACLevelScriptActor::AddDigArea(FVector DigCentroid, float HoleRadius)
{
	FSphere* NewSphere = new FSphere(DigCentroid, HoleRadius);
	PlanetHoles.Add(NewSphere);
}
