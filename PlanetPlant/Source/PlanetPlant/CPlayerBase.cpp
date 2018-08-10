	// Fill out your copyright notice in the Description page of Project Settings.

#include "CPlayerBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

#include "CPlayerBase_MovementComponent.h"


// Sets default values
ACPlayerBase::ACPlayerBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Create Components: */
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh"); // "Mesh" is the internal engine name, not the display name
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	/* Create Hierarchy: */
	RootComponent = Mesh;
	SpringArm->SetupAttachment(Mesh);
	Camera->SetupAttachment(SpringArm);
	
	//SpringArm->bEnableCameraLag = true;
	//SpringArm->CameraLagSpeed = 3.0f;

	MoveSpeed = 10;

	/* Create Custom Movement Component: */
	CustomMovementComponent = CreateDefaultSubobject<UCPlayerBase_MovementComponent>(TEXT("CustomMovementComponent"));
	CustomMovementComponent->UpdatedComponent = RootComponent;
}

// Called when the game starts or when spawned
void ACPlayerBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACPlayerBase::Tick(float DeltaSeconds)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(CurrentForwardSpeed));
	//const FVector LocalMoveDist = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);
	//AddActorLocalOffset(LocalMoveDist, true);

	Super::Tick(DeltaSeconds);
}

// Called to bind functionality to input
void ACPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Lateral_Axis", this, &ACPlayerBase::MoveLateralInput);
	PlayerInputComponent->BindAxis("Forward_Axis", this, &ACPlayerBase::MoveForwardInput);
	//PlayerInputComponent->BindAxis("Turn", this, &ACPlayerBase::Turn);
}

UPawnMovementComponent* ACPlayerBase::GetMovementComponent() const
{
	return CustomMovementComponent;
}

void ACPlayerBase::MoveForwardInput(float AxisValue) {
	//bool bHasInput = !FMath::IsNearlyEqual(Value, 0.f); // Check for significant input.
	//float AccelerationCur = bHasInput ? (Value * Acceleration) : ((-0.5f * FMath::Sign(Acceleration)) * Acceleration);
	//float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * AccelerationCur);
	//CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);

	if (CustomMovementComponent && (CustomMovementComponent->UpdatedComponent == RootComponent))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Forward");
		CustomMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue * MoveSpeed);
	}

	//AddMovementInput(GetActorForwardVector(), AxisValue*MoveSpeed, false);
}

void ACPlayerBase::MoveLateralInput(float AxisValue) {
	if (CustomMovementComponent && (CustomMovementComponent->UpdatedComponent == RootComponent))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Lateral");
		CustomMovementComponent->AddInputVector(GetActorRightVector() * AxisValue * MoveSpeed);
	}
}
