// Fill out your copyright notice in the Description page of Project Settings.

#include "CCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CMultiTool.h"
#include "CPickupActor.h"
#include "CGameMode.h"
#include "CPlayerState.h"

// Sets default values
ACCharacterBase::ACCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm->bUsePawnControlRotation = false; // false: Kevin VanHorn [9.6.18]

	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20.0f;

	ToolAttachSocketName = "MultiTool_Socket";

	ToolMode = EToolMode::Weapon; // Starting ToolMode
	//CurToolModeCounter = (uint8)EToolMode::Weapon;
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
	WeaponTool = GetWorld()->SpawnActor<ACMultiTool>(WeaponToolClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (WeaponTool) {
		WeaponTool->SetOwner(this);
		WeaponTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ToolAttachSocketName);
		WeaponTool->Activate();
	}

	DefaultTool = GetWorld()->SpawnActor<ACMultiTool>(DefaultToolClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (DefaultTool) {
		DefaultTool->SetOwner(this);
		DefaultTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ToolAttachSocketName);
		DefaultTool->Deactivate();
	}
	
	if (WeaponTool) {
		CurrentTool = WeaponTool; // WeaponTool
	}


	// Set Gamemode reference:
	GameMode = Cast<ACGameMode>(GetWorld()->GetAuthGameMode());
}

void ACCharacterBase::MoveForward(float AxisValue)
{
	AddForwardMovementInput(AxisValue);
}

void ACCharacterBase::MoveRight(float AxisValue)
{
	AddRightMovementInput(AxisValue);
}

void ACCharacterBase::BeginZoom() {
	bWantsToZoom = true;
}

void ACCharacterBase::EndZoom() {
	bWantsToZoom = false;
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
	CurrentTool->Activate();
}

void ACCharacterBase::SwitchTool()
{
	/*CurToolModeCounter++;
	if (CurToolModeCounter > (uint8)EToolMode::_Last) {
		CurToolModeCounter = 0;
	}

	EToolMode NextMode = static_cast<EToolMode>(CurToolModeCounter);

	SwitchToolMode(NextMode);*/

	if (ToolMode == EToolMode::Default) {
		SwitchToolMode(EToolMode::Weapon);
	}
	else if (ToolMode == EToolMode::Weapon) {
		SwitchToolMode(EToolMode::Default);
	}

	UE_LOG(LogTemp, Warning, TEXT("SWITCH MODE"));
}

// Called every frame
void ACCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: Make in coroutine
	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV; // Move this to a coroutine on input event. 
	float NewFOV = FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	Camera->SetFieldOfView(NewFOV);
}

// Called to bind functionality to input
void ACCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACCharacterBase::MoveForward); // Directional Input
	PlayerInputComponent->BindAxis("MoveRight", this, &ACCharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ACCharacterBase::_AddCameraPitchInput); // Calls Pawn built-in function.
	PlayerInputComponent->BindAxis("Turn", this, &ACCharacterBase::_AddCameraYawInput);
	
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