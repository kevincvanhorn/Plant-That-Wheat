
// Credit: UsableACtor:Tom Looman https://www.tomlooman.com/tutorial-usableactor-system-in-c/
#include "CDefaultTool.h"
#include "Public/MultiTool/CDefaultTool.h"
#include "CUsableActor.h"
#include "PlantThatWheat.h"

ACDefaultTool::ACDefaultTool() {
	PrimaryActorTick.bCanEverTick = true;
	bScanForUsables = true;

	MaxUseDistance = 800;
	bHasNewFocus = true; // Can focus on a new usable. 
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
	ACUsableActor* Usable = GetUsableInView();
	if (Usable)
	{
		Usable->OnUsed(this);
	}
}

void ACDefaultTool::Activate()
{
	Super::Activate();

	bHasNewFocus = true; // Can focus on a new usable. 
	bScanForUsables = false;
}

void ACDefaultTool::Deactivate()
{
	Super::Deactivate();
	bScanForUsables = false;
}
