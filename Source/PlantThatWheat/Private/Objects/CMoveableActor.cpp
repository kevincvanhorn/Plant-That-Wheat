

#include "CMoveableActor.h"
#include "CMultiTool.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "CustomPhysicsActor.h"
#include "CCharacterBase.h"
#include "Components/StaticMeshComponent.h"
#include "CDefaultTool.h"

#include "Components/ArrowComponent.h"

ACMoveableActor::ACMoveableActor() {
	bIsBeingHeld = false;
	PrimaryActorTick.bCanEverTick = true;

	HoldLocation = FVector::ZeroVector;
	PawnOffset = FVector(0, 500, 50);

	Owner = nullptr;
	bOutlineEnabled = true;

	bValidToolMode = true;
}

void ACMoveableActor::Tick(float DeltaTime)
{
	/*
	if (Owner && bIsBeingHeld) {
		HoldLocation = Owner->GetActorLocation();
		FRotator Rotation = Owner->GetForwardArrowComponent()->GetComponentRotation();
		ForwardVector = Owner->GetForwardArrowComponent()->GetForwardVector();
		FVector UpVector = Owner->GetForwardArrowComponent()->GetUpVector();

		SetActorLocationAndRotation(HoldLocation + ForwardVector * PawnOffset.Y + UpVector*PawnOffset.Z, Rotation);
	}
	else { return; }*/
}

bool ACMoveableActor::OnUsed_Implementation(ACMultiTool * Tool)
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
	else {
		PickUp();
	}
	return true;
}

bool ACMoveableActor::StartFocus_Implementation()
{
	if (bOutlineEnabled && bValidToolMode)
		MeshComponent->SetRenderCustomDepth(true);
	return true;
}

bool ACMoveableActor::EndFocus_Implementation()
{
	if (bOutlineEnabled && bValidToolMode)
		MeshComponent->SetRenderCustomDepth(false);
	return true;
}

void ACMoveableActor::DisableOutlines()
{
	if (bOutlineEnabled) {
		MeshComponent->SetRenderCustomDepth(false);
	}
}

void ACMoveableActor::External_DropMoveable()
{
	if (MultiTool) {
		ACDefaultTool* DefaultTool = Cast<ACDefaultTool>(MultiTool);
		if (!DefaultTool) return;
		SetDown();
		DefaultTool->bIsHoldingMoveable = false;
		DefaultTool->bScanForUsables = true;
	}
}

void ACMoveableActor::PickUp() {
	UE_LOG(LogTemp, Warning, TEXT("PICK UP"));
	bIsBeingHeld = true;
	//MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	
	//FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	
	//MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	if (Owner) {
		//Owner->SetAttachmentOffset(PawnOffset);
		//AttachToComponent(Owner->AttachObjectComp, AttachRules); // Offset the component of the Character to attach to.
		//Owner->Try
		Owner->TryPickupMoveable(PawnOffset, GetMesh());
	}

	// Disable Outline
	if (bOutlineEnabled) {
		MeshComponent->SetRenderCustomDepth(false);
	}
}

void ACMoveableActor::SetDown() {
	bIsBeingHeld = false;

	TSet<AActor*> OverlappingActors;

	//MeshComponent->SetSimulatePhysics(true);
	MeshComponent->GetOverlappingActors(OverlappingActors);
	if (OverlappingActors.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("ACMoveableActor: SetDown(), @NumOverlappingActors = %d"), OverlappingActors.Num());
	}
	else {
		MeshComponent->SetSimulatePhysics(false);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// Disable Outline
	if (bOutlineEnabled) {
		MeshComponent->SetRenderCustomDepth(false);
	}
}