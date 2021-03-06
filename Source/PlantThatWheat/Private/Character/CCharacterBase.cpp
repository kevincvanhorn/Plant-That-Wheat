// Fill out your copyright notice in the Description page of Project Settings.

#include "CCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CMultiTool.h"
#include "CPickupActor.h"
#include "CGameMode.h"
#include "CPlayerState.h"
#include "CShovelTool.h"

#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"

#include "CDefaultTool.h"
#include "CPlantingTool.h"
#include "CGunTool.h"
#include "CHarvestTool.h"
#include "CSeedThrower.h"
#include "CDigTool.h"
#include "CWateringTool.h"

#include "Components/CapsuleComponent.h"	
#include "Components/ArrowComponent.h"

#include "CustomPawn.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Engine/Classes/PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values
ACCharacterBase::ACCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm->bUsePawnControlRotation = false; // false: Kevin VanHorn [9.6.18]

	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20.0f;

	ToolAttachSocketName = "MultiTool_Socket";

	ToolMode = EToolMode::Default; // Starting ToolMode
	//CurToolModeCounter = (uint8)EToolMode::Weapon;

	AttachObjectComp = CreateDefaultSubobject<USceneComponent>(TEXT("AttachObjectComp"));
	if (AttachObjectComp) {
		AttachObjectComp->SetupAttachment(PawnMesh);
	}

	PhysicsHandleComp = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComp"));
}

// Called when the game starts or when spawned
void ACCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	DefaultFOV = Camera->FieldOfView;

	// Spawn a Default Tool:
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Set Tool Attachments:
	WeaponTool = GetWorld()->SpawnActor<ACGunTool>(WeaponToolClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (WeaponTool) {
		WeaponTool->SetOwner(this);
		WeaponTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ToolAttachSocketName);
		WeaponTool->Deactivate();
	}

	DefaultTool = GetWorld()->SpawnActor<ACDefaultTool>(DefaultToolClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (DefaultTool) {
		DefaultTool->SetOwner(this);
		DefaultTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ToolAttachSocketName);
		DefaultTool->Deactivate();
	}

	ShovelTool = GetWorld()->SpawnActor<ACShovelTool>(ShovelToolClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (ShovelTool) {
		ShovelTool->SetOwner(this);
		ShovelTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		ShovelTool->Deactivate();
	}

	PlantingTool = GetWorld()->SpawnActor<ACPlantingTool>(PlantingToolClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (PlantingTool) {
		PlantingTool->SetOwner(this);
		PlantingTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ToolAttachSocketName);
		PlantingTool->Deactivate();
	}

	SeedTool = GetWorld()->SpawnActor<ACSeedThrower>(SeedToolClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (SeedTool) {
		SeedTool->SetOwner(this);
		SeedTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ToolAttachSocketName);
		SeedTool->Deactivate();
	}

	DigTool = GetWorld()->SpawnActor<ACDigTool>(DigToolClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (DigTool) {
		DigTool->SetOwner(this);
		DigTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ToolAttachSocketName);
		DigTool->Deactivate();
	}

	WateringTool = GetWorld()->SpawnActor<ACWateringTool>(WateringToolClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (WateringTool) {
		WateringTool->SetOwner(this);
		//WateringTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ToolAttachSocketName);
		WateringTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		WateringTool->Deactivate();
	}

	HarvestTool = GetWorld()->SpawnActor<ACHarvestTool>(HarvestToolClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (HarvestTool) {
		HarvestTool->SetOwner(this);
		HarvestTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ToolAttachSocketName);
		HarvestTool->Deactivate();
	}
	
	if (DefaultTool) {
		CurrentTool = DefaultTool;
		CurrentTool->Activate();
	}

	// Set Gamemode reference:
	GameMode = Cast<ACGameMode>(GetWorld()->GetAuthGameMode());
}

void ACCharacterBase::MoveForward(float AxisValue)
{
	AddForwardMovementInput(AxisValue);

}

// Strafe Right
void ACCharacterBase::MoveRight(float AxisValue)
{
	AddRightMovementInput(AxisValue);
}

void ACCharacterBase::BeginZoom() {
	bWantsToZoom = !bWantsToZoom;//true; // Changed to toggle for tools to interact with zoom.
}

void ACCharacterBase::EndZoom() {
	bWantsToZoom = !bWantsToZoom;//false;
}

void ACCharacterBase::Fire()
{
	if (CurrentTool) {
		CurrentTool->Interact();
	}
}

void ACCharacterBase::SwitchToolMode(EToolMode NewToolMode)
{
	CurrentTool->Deactivate();
	ToolMode = NewToolMode;

	if (ToolMode == EToolMode::Weapon) {
		CurrentTool = WeaponTool;
		UE_LOG(LogTemp, Warning, TEXT("WEAPON MODE"));
	}
	else if (ToolMode == EToolMode::Default) {
		CurrentTool = DefaultTool;
		UE_LOG(LogTemp, Warning, TEXT("DEFAULT MODE"));
	}
	else if (ToolMode == EToolMode::Shovel) {
		UE_LOG(LogTemp, Warning, TEXT("SHOVEL MODE"));
		CurrentTool = ShovelTool;
	}
	else if (ToolMode == EToolMode::Planting) {
		UE_LOG(LogTemp, Warning, TEXT("PLANTING MODE"));
		CurrentTool = PlantingTool;
	}
	else if (ToolMode == EToolMode::Harvest) {
		UE_LOG(LogTemp, Warning, TEXT("HARVEST MODE"));
		CurrentTool = HarvestTool;
	}
	else if (ToolMode == EToolMode::Seed) {
		UE_LOG(LogTemp, Warning, TEXT("SEED MODE"));
		CurrentTool = SeedTool;
	}
	else if (ToolMode == EToolMode::Dig) {
		UE_LOG(LogTemp, Warning, TEXT("DIG MODE"));
		CurrentTool = DigTool;
	}
	else if (ToolMode == EToolMode::Watering) {
		UE_LOG(LogTemp, Warning, TEXT("WATERING MODE"));
		CurrentTool = WateringTool;
	}
	CurrentTool->Activate();
}

void ACCharacterBase::SwitchTool()
{
	CurToolModeIndex++;
	if (CurToolModeIndex >= ActiveTools.Num()) {
		CurToolModeIndex = 0;
	}

	EToolMode NextMode = ActiveTools[CurToolModeIndex];
	SwitchToolMode(NextMode);

	UE_LOG(LogTemp, Warning, TEXT("SWITCH MODE"));
}

void ACCharacterBase::SetAttachmentOffset(FVector & Offset) const
{
	if (AttachObjectComp) {
		AttachObjectComp->SetRelativeLocation(Offset);
	}
}

// Called every frame
void ACCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: Make in coroutine
	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV; // Move this to a coroutine on input event. 
	float NewFOV = FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	// DEBUG: Actor direction for compass.
	/*
	FVector Axis = GetActorLocation();
	float Angle = GetSpringArm()->RelativeRotation.Yaw;
	FRotator Rotator = UKismetMathLibrary::RotatorFromAxisAndAngle(Axis, Angle);
	
	FVector Rot = Rotator.Vector();

	FVector TopPoint = FVector(0, 0, 0) + FVector(0,0,1587); // TODO: Use planet center and radius.
	FVector VToPole = TopPoint - GetActorLocation();

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(Axis*200), FColor::Blue, false);
	
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(ForwardArrowComponent->GetForwardVector()*300), FColor::Purple, false);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(VToPole*300), FColor::Yellow, false);
	*/

	Camera->SetFieldOfView(NewFOV); // TODO: Move to coroutine.

	UpdateGrabbedLoc();	
}

// Called to bind functionality to input
void ACCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACCharacterBase::MoveForward); // Directional Input
	PlayerInputComponent->BindAxis("MoveRight", this, &ACCharacterBase::MoveRight);		// Strafe Right

	PlayerInputComponent->BindAxis("LookUp", this, &ACCharacterBase::_AddCameraPitchInput); // Calls Pawn built-in function.
	PlayerInputComponent->BindAxis("LookRight", this, &ACCharacterBase::_AddCameraYawInput); // Camera turn right
	//PlayerInputComponent->BindAxis("Turn", this, &ACCharacterBase::_AddTurnInput);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACustomPawn::Jump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ACCharacterBase::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ACCharacterBase::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACCharacterBase::Fire);

	PlayerInputComponent->BindAction("SwitchTool", IE_Pressed, this, &ACCharacterBase::SwitchTool);
}

// Used for MultiTool
FVector ACCharacterBase::GetPawnViewLocation() const
{
	if (Camera) {
		return	Camera->GetComponentLocation();
	}

	return Super::GetPawnViewLocation(); // Returns location from pawn BaseEyeHeight.
}

void ACCharacterBase::_AddCameraPitchInput(float Val) {
	AddCameraPitchInput(1, Val);
}

void ACCharacterBase::_AddCameraYawInput(float Val) {
	AddCameraYawInput(1, Val);
}

void ACCharacterBase::_AddTurnInput(float Val)
{
	//AddPawnTurnInput(3.5f, Val);
}


void ACCharacterBase::OnPickupItem(ACPickupActor * Pickup)
{
	// Consume Pickup.
	Pickup->Destroy();
	
	// If Wheat Pickup:
	//ACGameMode* GM = Cast<ACGameMode>(GetWorld()->GetAuthGameMode());
	//if (GM){
	//	GM->OnPlayerCollectWheat.Broadcast();	
	//}
}

void ACCharacterBase::TryPickupMoveable(FVector & Offset, UPrimitiveComponent * MoveableMesh)
{
	if (PhysicsHandleComp) {
		MoveableOffset = Offset;
		//SetAttachmentOffset(Offset);

		//PhysicsHandleComp->TargetTransform = GetActorTransform();
		PhysicsHandleComp->GrabComponent(MoveableMesh, NAME_None, GetActorLocation(), true);
		MoveableMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		bIsPhysicsHandleActive = true;
	}
}

void ACCharacterBase::UpdateGrabbedLoc()
{
	if (bIsPhysicsHandleActive) {
		FRotator Rotation = GetForwardArrowComponent()->GetComponentRotation();
		FVector ForwardVector = GetForwardArrowComponent()->GetForwardVector();
		FVector UpVector = GetForwardArrowComponent()->GetUpVector();

		PhysicsHandleComp->SetTargetLocationAndRotation(GetActorLocation() + ForwardVector * MoveableOffset.Y + UpVector * MoveableOffset.Z, Rotation);

		//UE_LOG(LogTemp, Warning, TEXT("OFFSET = %s"), *MoveableOffset.ToString());

		//PhysicsHandleComp->SetTargetLocationAndRotation(AttachObjectComp->GetComponentLocation(), AttachObjectComp->GetComponentRotation());
	}	
}

void ACCharacterBase::ReleaseGrabbedComp()
{
	bIsPhysicsHandleActive = false;
	if (PhysicsHandleComp) {
		PhysicsHandleComp->ReleaseComponent();
	}
}

void ACCharacterBase::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	if (Camera) {
		OutLocation	= Camera->GetComponentLocation();
		OutRotation	= Camera->GetComponentRotation();
	}
	else {
		// This is the original implementation from AActor.
		// [9.8.18] Custom gravity movement controller does not update AActor Location/Rotation - using camera instead.
		// TODO: Find the alternative in the custom movement controller for actor loc/rot.
		OutLocation = GetActorLocation();
		OutRotation = GetActorRotation();
	}
}