// Fill out your copyright notice in the Description page of Project Settings.

#include "CCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CMultiTool.h"

// Sets default values
ACCharacter::ACCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true; // Rotate springArm based on Character rotation. 
	SpringArmComp->SetupAttachment(RootComponent);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20.0f;
	
	ToolAttachSocketName = "MultiTool_Socket";
}

// Called when the game starts or when spawned
void ACCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComp->FieldOfView;

	// Spawn a Default Tool:
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentTool = GetWorld()->SpawnActor<ACMultiTool>(StarterToolClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (CurrentTool) {
		CurrentTool->SetOwner(this);
		CurrentTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ToolAttachSocketName);
	}
}

void ACCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);

}

void ACCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);

}

void ACCharacter::BeginCrouch()
{
	Crouch();
}

void ACCharacter::EndCrouch()
{
	UnCrouch();
}

void ACCharacter::BeginZoom() {
	bWantsToZoom = true;
}

void ACCharacter::EndZoom() {
	bWantsToZoom = false;
}

void ACCharacter::Fire()
{
	if (CurrentTool) {
		CurrentTool->Fire();
	}
}

// Called every frame
void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV; // Move this to a coroutine on input event. 
	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	CameraComp->SetFieldOfView(NewFOV);
}

// Called to bind functionality to input
void ACCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACCharacter::MoveForward);// Directional Input
	PlayerInputComponent->BindAxis("MoveRight", this, &ACCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ACCharacter::AddControllerPitchInput); // Calls Pawn built-in function.
	PlayerInputComponent->BindAxis("Turn", this, &ACCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ACCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ACCharacter::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACCharacter::Fire);
}

FVector ACCharacter::GetPawnViewLocation() const
{
	if (CameraComp) {
		return	CameraComp->GetComponentLocation();
	}
	
	return Super::GetPawnViewLocation(); // Returns location from pawn BaseEyeHeight.
}
