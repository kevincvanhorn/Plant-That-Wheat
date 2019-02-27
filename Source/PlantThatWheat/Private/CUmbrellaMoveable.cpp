

#include "CUmbrellaMoveable.h"
#include "Components/StaticMeshComponent.h"
#include "CCharacterBase.h"
#include "Engine/DecalActor.h"
#include "CMoveableActor.h"

#include "CMultiTool.h"
#include "CDefaultTool.h"

#include "DrawDebugHelpers.h"

#include "Components/ArrowComponent.h"

#include "PlantThatWheat.h"
#include "Engine/Public/TimerManager.h"

#include "Engine/Classes/Kismet/KismetMathLibrary.h"

#include "Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"

ACUmbrellaMoveable::ACUmbrellaMoveable() {

}

void ACUmbrellaMoveable::BeginPlay()
{
	Super::BeginPlay();

	FTransform TDecal(FRotator::ZeroRotator, FVector::ZeroVector, GetActorScale()*0.25f); // where .25 is the size ratio of the placement decal 

	PlacementDecal = GetWorld()->SpawnActor<ADecalActor>(DecalClass, TDecal);
	PlacementDecal->SetActorHiddenInGame(true);
}

bool ACUmbrellaMoveable::OnUsed_Implementation(ACMultiTool * Tool)
{
	if (bOutlineEnabled && !bValidToolMode)
		MeshComponent->SetRenderCustomDepth(false);

	if (Tool) {
		MultiTool = Tool;
		if (Tool->MyOwner) {
			Owner = Tool->MyOwner;
		}
	}
	else { return false; }

	if (bIsBeingHeld) {
		SetDown();
	}
	else if (CurState == EMoveableState::MS_Rotating) {
		OnFinishManualRot();
	}
	else {
		PickUp();
	}
	return true;
}

void ACUmbrellaMoveable::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// Rotate Umbrella:
	if (CurState == EMoveableState::MS_Rotating && Owner) {

		float DeltaX, DeltaY;
		PlayerController->GetInputMouseDelta(DeltaX, DeltaY);

		FRotator CurRot = MeshComponent->GetComponentRotation();

		float DesiredY = FMath::Clamp(DeltaX*MouseRotFactor + CurRot.Roll, MeshRot.Roll - 90, MeshRot.Roll + 90);
		float DesiredX = FMath::Clamp(-1 * DeltaY*MouseRotFactor + CurRot.Pitch, MeshRot.Pitch - 90, MeshRot.Pitch + 90);

		FRotator DesiredRot(-1*DeltaY*MouseRotFactor, 0, DeltaX*MouseRotFactor);
		MeshComponent->SetRelativeRotation(CurRot + DesiredRot);

		/*
		FRotator CurRot = MeshComponent->GetComponentRotation();

		CurRot.Roll = (CurRot.Roll > 180)? 180 - CurRot.Roll : CurRot.Roll;
		CurRot.Pitch = (CurRot.Pitch > 180) ? 180 - CurRot.Pitch : CurRot.Pitch;

		float DeltaX, DeltaY;
		PlayerController->GetInputMouseDelta(DeltaX, DeltaY);

		float DesiredY = FMath::Clamp(DeltaX*MouseRotFactor + CurRot.Roll, MeshRot.Roll - 90, MeshRot.Roll + 90);
		float DesiredX = FMath::Clamp(-1 * DeltaY*MouseRotFactor + CurRot.Pitch, MeshRot.Pitch - 90, MeshRot.Pitch + 90);
		// Normalize rotation value from 0-360
		DesiredX = FMath::Fmod(DesiredX, 360);
		DesiredY = FMath::Fmod(DesiredY,360);
		if (DesiredX < 0) { DesiredX += 360; }
		if (DesiredY < 0) { DesiredY += 360; }

		FRotator MouseRot(DesiredX, MeshRot.Yaw, DesiredY);
		MeshComponent->SetRelativeRotation(MouseRot);
		*/
	}
	
	if (CurState == EMoveableState::MS_Held) {
		TraceToSurface();
	}
}

void ACUmbrellaMoveable::PickUp() {
	if (CurState == EMoveableState::MS_Rotating) return;

	Super::PickUp();
	PlacementDecal->SetActorHiddenInGame(false);

	CurState = EMoveableState::MS_Held;

	// This should be in an init function.
	/*if (Owner) {
		FViewport* View = CastChecked<ULocalPlayer>(Cast<APlayerController>(Owner->Controller))->ViewportClient->Viewport;
		if (View) {
			Viewport = View;
		}
	}*/
	PlayerController = Cast<APlayerController>(Owner->Controller);
	//Viewport = (GetWorld()->GetFirstLocalPlayerFromController())->ViewportClient->Viewport;
}

void ACUmbrellaMoveable::SetDown()
{
	Owner->ReleaseGrabbedComp();
	bIsBeingHeld = false;

	//MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block); // TODO put this after second click
	
	// Releases the object w/ physics if the object is released in the air.
	MeshComponent->SetSimulatePhysics(false);
	//MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Disable Outline
	if (bOutlineEnabled) {
		MeshComponent->SetRenderCustomDepth(true);
	}

	if (GetWorldTimerManager().IsTimerActive(FUmbrellaPlaceHandle)) return;

	TimeSincePlace = 0;

	DistToDecal = FVector::Dist(MeshComponent->GetComponentLocation(), PlacementDecal->GetActorLocation());
	SpeedToDecal = DistToDecal / TimeToDecal;
	MeshRot = GetActorRotation();

	GetWorld()->GetTimerManager().SetTimer(FUmbrellaPlaceHandle, this, &ACUmbrellaMoveable::MoveToSurface, .0125f, true, 0.0f);
}

void ACUmbrellaMoveable::MoveToSurface() {
	TimeSincePlace += .0125;

	//MeshComponent->SetWorldLocation(FMath::VInterpConstantTo(MeshComponent->GetComponentLocation(), PlacementDecal->GetActorLocation(), .0125, SpeedToDecal));
	
	MeshComponent->SetWorldLocation(FMath::VInterpTo(MeshComponent->GetComponentLocation(), PlacementDecal->GetActorLocation(), .0125, SpeedToDecal));

	const FVector Dist = PlacementDecal->GetActorLocation() - MeshComponent->GetComponentLocation(); // Target - Current
	if (Dist.SizeSquared() < KINDA_SMALL_NUMBER) {
		//PlacementDecal->SetActorHiddenInGame(true);
		bCanManualRotate = true;
		CurState = EMoveableState::MS_Rotating;
		GetWorld()->GetTimerManager().ClearTimer(FUmbrellaPlaceHandle);
	}

	/*if (FMath::IsNearlyZero(FVector::Dist(MeshComponent->GetComponentLocation(), PlacementDecal->GetActorLocation())), .00000001f) {
		SetDownTimeElapsed = 0;
		PlacementDecal->SetActorHiddenInGame(true);
		GetWorld()->GetTimerManager().ClearTimer(FUmbrellaPlaceHandle);
	}*/
	
	UE_LOG(LogTemp, Warning, TEXT("Time elapsed! %f"), GetWorldTimerManager().GetTimerElapsed(FUmbrellaPlaceHandle));

	/*if (TimeSincePlace >= TimeToDecal) {
		PlacementDecal->SetActorHiddenInGame(true);
		GetWorld()->GetTimerManager().ClearTimer(FUmbrellaPlaceHandle);
	}*/
}

void ACUmbrellaMoveable::OnFinishManualRot()
{
	if (CurState == EMoveableState::MS_Rotating) {
		ACDefaultTool* DefaultTool = Cast<ACDefaultTool>(MultiTool);
		if (!DefaultTool) return;

		PlacementDecal->SetActorHiddenInGame(true);
		bCanManualRotate = false;
		CurState = EMoveableState::MS_Static;
		DefaultTool->bIsHoldingMoveable = false;
	}
}

void ACUmbrellaMoveable::TraceToSurface()
{
	float SingleTraceDist = 800;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false; // Trace against each triangle of the mesh - ie. exact location. 
	QueryParams.bReturnPhysicalMaterial = true;

	FVector TraceStart = GetActorLocation();
	FVector TraceDirection = (-1 * TraceStart).GetSafeNormal(); // PlanetLoc - TraceStart
	TraceStart += TraceDirection * -1 * 200; // Move up by normal so that the planet is always hit.
	FVector TraceEnd = TraceStart + (TraceDirection * SingleTraceDist); // Trace EndLocation

	FHitResult Hit; // Struct filled with hit data.
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Cyan, false, 2);
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, COLLISION_DIGTRACE, QueryParams)) {
		//UE_LOG(LogTemp, Warning, TEXT("Umbrella Placement hit! %s"), *Hit.ToString());

		FRotator Rot = TraceDirection.ToOrientationRotator();
		Rot.Roll = MeshComponent->GetComponentRotation().Yaw;
		PlacementDecal->SetActorLocationAndRotation(Hit.Location, Rot);
	}
}