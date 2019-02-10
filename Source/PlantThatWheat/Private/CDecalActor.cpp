

#include "CDecalActor.h"


ACDecalActor::ACDecalActor() {
	bIsAnimPlaying = false;
}

bool ACDecalActor::UpdateTransform(FVector & NewLoc, FRotator& NewNormal)
{
	if (!bIsAnimPlaying) {
		SetActorLocation(NewLoc);
		SetActorRotation(NewNormal);

		this->OnStartAnimation();
		return true;
	}

	return false;
}

void ACDecalActor::OnFinshAnimation()
{
	bIsAnimPlaying = false;
}

void ACDecalActor::OnStartAnimation_Implementation()
{
	bIsAnimPlaying = true;
}
