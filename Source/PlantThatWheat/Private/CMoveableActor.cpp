

#include "CMoveableActor.h"
#include "CMultiTool.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"


ACMoveableActor::ACMoveableActor() {
	bIsBeingHeld = false;
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = MeshComp;
	HoldLocation = FVector::ZeroVector;
}

void ACMoveableActor::Tick(float DeltaTime)
{
	if (bIsBeingHeld) {
		SetActorLocation(HoldLocation);
	}
}

bool ACMoveableActor::OnUsed_Implementation(ACMultiTool * Tool)
{
	if (Tool) {
		if (Tool->MyOwner) {
			HoldLocation = Tool->MyOwner->GetPawnViewLocation();
		}
	}
	else { return false; }

	if (bIsBeingHeld) {
		SetDown();
	}
	else {
		PickUp();
	}

	bIsBeingHeld = !bIsBeingHeld;

	return true;
}

bool ACMoveableActor::StartFocus_Implementation()
{
	return false;
}

bool ACMoveableActor::EndFocus_Implementation()
{
	return false;
}

void ACMoveableActor::PickUp() {

}

void ACMoveableActor::SetDown() {

}