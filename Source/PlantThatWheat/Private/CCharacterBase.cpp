// Fill out your copyright notice in the Description page of Project Settings.

#include "CCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CMultiTool.h"

// Sets default values
ACCharacterBase::ACCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm->bUsePawnControlRotation = true; // Rotate springArm based on Character rotation. 

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20.0f;

	ToolAttachSocketName = "MultiTool_Socket";
}

// Called when the game starts or when spawned
void ACCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = Camera->FieldOfView;

	// Spawn a Default Tool:
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentTool = GetWorld()->SpawnActor<ACMultiTool>(StarterToolClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (CurrentTool) {
		CurrentTool->SetOwner(this);
		CurrentTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ToolAttachSocketName);
	}
}

void ACCharacterBase::MoveForward(float AxisValue)
{
	//AddMovementInput(GetActorForwardVector() * AxisValue);
	AddForwardMovementInput(AxisValue);
}

void ACCharacterBase::MoveRight(float AxisValue)
{
	//AddMovementInput(GetActorRightVector() * AxisValue);
	AddRightMovementInput(AxisValue);
}

/*void ACCharacterBase::BeginCrouch()
{
	Crouch();
}

void ACCharacterBase::EndCrouch()
{
	UnCrouch();
}*/

void ACCharacterBase::BeginZoom() {
	bWantsToZoom = true;
}

void ACCharacterBase::EndZoom() {
	bWantsToZoom = false;
}

void ACCharacterBase::Fire()
{
	if (CurrentTool) {
		CurrentTool->Fire();
	}
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

	PlayerInputComponent->BindAxis("MoveForward", this, &ACCharacterBase::MoveForward);// Directional Input
	PlayerInputComponent->BindAxis("MoveRight", this, &ACCharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ACCharacterBase::_AddCameraPitchInput); // Calls Pawn built-in function.
	PlayerInputComponent->BindAxis("Turn", this, &ACCharacterBase::_AddCameraYawInput);

	//PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACCharacterBase::BeginCrouch);
	//PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACCharacterBase::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACustomPawn::Jump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ACCharacterBase::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ACCharacterBase::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACCharacterBase::Fire);
}

// Used for MultiTool
FVector ACCharacterBase::GetPawnViewLocation() const
{
	if (Camera) {
		return	Camera->GetComponentLocation();
	}

	return Super::GetPawnViewLocation(); // Returns location from pawn BaseEyeHeight.
}

void ACCharacterBase::_AddCameraPitchInput(float AxisValue)
{
	AddCameraPitchInput(AxisValue);
}

void ACCharacterBase::_AddCameraYawInput(float AxisValue)
{
	AddCameraYawInput(AxisValue);
}




