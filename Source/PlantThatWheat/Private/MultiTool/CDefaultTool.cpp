
// Credit: UsableACtor:Tom Looman https://www.tomlooman.com/tutorial-usableactor-system-in-c/
#include "CDefaultTool.h"
#include "CUsableActor.h"
#include "CMoveableActor.h"
#include "PlantThatWheat.h"
#include "CMoveableActor.h"
#include "CGroundSection.h"

#include "Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Classes/Engine/World.h"

#include "CUmbrellaMoveable.h"

ACDefaultTool::ACDefaultTool() {
	PrimaryActorTick.bCanEverTick = true;
	bScanForUsables = true;

	MaxUseDistance = 1200;
	bHasNewFocus = true; // Can focus on a new usable. 
	bIsHoldingMoveable = false;
	MoveableActor = NULL;
}

void ACDefaultTool::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bScanForUsables && OwnerController) {
		ACUsableActor* Usable = GetUsableInView();

		// End Focus:
		if (FocusedUsableActor != Usable)
		{
			if (FocusedUsableActor)
			{
				FocusedUsableActor->EndFocus();
			}

			bHasNewFocus = true;
		}

		// Assign new Focus:
		FocusedUsableActor = Usable;

		// Start Focus:
		if (Usable)
		{
			if (bHasNewFocus)
			{
				MoveableActor = Cast<ACMoveableActor>(Usable);
				if (MoveableActor) {
					MoveableActor->SetValidToolMode(true);
				}

				GroundSection = Cast<ACGroundSection>(Usable);

				Usable->StartFocus();
				bHasNewFocus = false;
			}
		}
	}
}

ACUsableActor * ACDefaultTool::GetUsableInView()
{
	FVector camLoc;
	FRotator camRot;

	if (OwnerController == NULL)
		return NULL;

	OwnerController->GetActorEyesViewPoint(camLoc, camRot);
	const FVector start_trace = camLoc;
	const FVector direction = camRot.Vector();
	const FVector end_trace = start_trace + (direction * MaxUseDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, start_trace, end_trace, COLLISION_DEFAULTTOOL, TraceParams);

	return Cast<ACUsableActor>(Hit.GetActor());
}

void ACDefaultTool::Interact() {
	// Primitive Grabbables:
	/*if (!bIsPhysicsHandleActive) {
		if (MyOwner) {
			if (MyOwner->GrabComponent()) {
				bIsPhysicsHandleActive = true;
			}
		}
	}
	else {
		bIsPhysicsHandleActive = false;
		MyOwner->ReleaseComponent();
	}*/
	
	// Set down moveable actor:
	if (MoveableActor && bIsHoldingMoveable) {
		MoveableActor->OnUsed(this);
		bScanForUsables = true;
		MoveableActor->OnFinishManualRot(); // Conditions are handled within umbrellamoveable - only like this because local enum
	}
	else {
		ACUsableActor* Usable = GetUsableInView();
		if (Usable)
		{
			// On pickup of Moveable Actor:
			MoveableActor = Cast<ACMoveableActor>(Usable);
			if (MoveableActor) {
				bIsHoldingMoveable = true;
				MoveableActor->SetValidToolMode(true);
				bScanForUsables = false;
			}
			Usable->OnUsed(this);
		}
	}
}

void ACDefaultTool::Activate()
{
	Super::Activate();

	bHasNewFocus = true; // Can focus on a new usable. 
	bScanForUsables = true;
}

void ACDefaultTool::Deactivate()
{
	Super::Deactivate();
	bScanForUsables = false;

	if (MoveableActor) {
		MoveableActor->SetValidToolMode(false);
		MoveableActor->DisableOutlines();
	}

	// Set Down Moveable:
	if (bIsHoldingMoveable) {
		MoveableActor->OnUsed(this);
		bIsHoldingMoveable = false;
	}
}
