

#include "CMoveableActor.h"
#include "CMultiTool.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "CustomPhysicsActor.h"

ACMoveableActor::ACMoveableActor() {
	bIsBeingHeld = false;
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = MeshComp;
	HoldLocation = FVector::ZeroVector;
	PawnOffset = 500;

	Owner = nullptr;
}

void ACMoveableActor::Tick(float DeltaTime)
{
	if (Owner && bIsBeingHeld) {
		HoldLocation = Owner->GetPawnViewLocation();
		FRotator Rotation = Owner->GetControlRotation();
		ForwardVector = Owner->GetActorForwardVector();
		FVector UpVector = Owner->GetActorUpVector();
		FVector LeftVector = Owner->GetActorRightVector();
		SetActorLocationAndRotation(HoldLocation + ForwardVector * PawnOffset + UpVector*50, Rotation);
	}
	else { return; }
}

bool ACMoveableActor::OnUsed_Implementation(ACMultiTool * Tool)
{
	if (Tool) {
		if (Tool->MyOwner) {
			Owner = Tool->MyOwner;
		}
	}
	else { return false; }

	if (bIsBeingHeld) {
		SetDown();
	}
	else {
		PickUp();
	}
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
	UE_LOG(LogTemp, Warning, TEXT("PICK UP"));
	bIsBeingHeld = true;
	MeshComponent->SetSimulatePhysics(false);
}

void ACMoveableActor::SetDown() {
	bIsBeingHeld = false;
	MeshComponent->SetSimulatePhysics(true);
}