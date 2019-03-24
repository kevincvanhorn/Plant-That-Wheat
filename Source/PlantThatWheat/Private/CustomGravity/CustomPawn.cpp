	// Copyright 2015 Elhoussine Mehnik (Mhousse1247). All Rights Reserved.
//******************* http://ue4resources.com/ *********************//

#include "CustomPawn.h"

#include "DrawDebugHelpers.h"

//ACharacter* a;

#include "CustomGravityPluginPrivatePCH.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"

// Sets default values
ACustomPawn::ACustomPawn() //const FObjectInitializer& ObjectInitializer //:Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule0"));
	if (CapsuleComponent)
	{
		CapsuleComponent->InitCapsuleSize(42.0f, 96.0f);
		CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
		CapsuleComponent->SetSimulatePhysics(true);
		CapsuleComponent->SetEnableGravity(false);
		RootComponent = CapsuleComponent;
		CapsuleComponent->GetBodyInstance()->COMNudge = FVector(0.0f, 0.0f, -96.0f);
		CapsuleComponent->SetLinearDamping(0.15f);
		CapsuleComponent->SetAngularDamping(100.0f);
		CapsuleComponent->SetNotifyRigidBodyCollision(true);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	if (SpringArm)
	{
		SpringArm->TargetArmLength = 600.0f;
		SpringArm->bDoCollisionTest = true;
		SpringArm->ProbeSize = 15.0f;
		SpringArm->ProbeChannel = ECollisionChannel::ECC_Camera;
		SpringArm->bUsePawnControlRotation = false; // false: Kevin VanHorn [9.6.18]
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bInheritRoll = true;
		SpringArm->SetupAttachment(GetMesh()); //CapsuleComponent
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera0"));
	if (Camera)
	{
		Camera->FieldOfView = 90.0f;
		Camera->SetupAttachment(SpringArm, SpringArm->SocketName);
	}

	PawnMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PawnMesh0"));
	if (PawnMesh)
	{
		//PawnMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		PawnMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		//Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
		PawnMesh->bCastDynamicShadow = true;
		PawnMesh->bAffectDynamicIndirectLighting = true;
		PawnMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		PawnMesh->SetCollisionProfileName(TEXT("CharacterMesh"));
		PawnMesh->SetGenerateOverlapEvents(false); // Kevin VanHorn: 9.1.18
		PawnMesh->SetNotifyRigidBodyCollision(false);
		PawnMesh->SetupAttachment(CapsuleComponent);
	}

	MovementComponent = CreateDefaultSubobject<UCustomMovementComponent>(TEXT("MovementComponent0"));
	if (MovementComponent)
	{
		MovementComponent->SetUpdatedComponent(CapsuleComponent);
		MovementComponent->SetComponentOwner(this);
	}

	GizmoRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("GizmoRootComponent0"));
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));

	if (RootSceneComponent) {
		RootSceneComponent->SetupAttachment(CapsuleComponent);
		RootSceneComponent->SetVisibility(true, true);
		RootSceneComponent->SetHiddenInGame(true, true);
	}

	if (GizmoRootComponent)
	{
		GizmoRootComponent->SetupAttachment(CapsuleComponent);

		ForwardArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrowComponent0"));
		if (ForwardArrowComponent)
		{
			ForwardArrowComponent->ArrowColor = FColor::Red;
			ForwardArrowComponent->bTreatAsASprite = true;
			ForwardArrowComponent->SetupAttachment(GizmoRootComponent);
			ForwardArrowComponent->bIsScreenSizeScaled = true;
			ForwardArrowComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		}

		RightArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArrowComponent0"));
		if (RightArrowComponent)
		{
			RightArrowComponent->ArrowColor = FColor::Green;
			RightArrowComponent->bTreatAsASprite = true;
			RightArrowComponent->SetupAttachment(GizmoRootComponent);
			RightArrowComponent->bIsScreenSizeScaled = true;
			RightArrowComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

		}

		UpArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("UpArrowtComponent0"));
		if (UpArrowComponent)
		{
			UpArrowComponent->ArrowColor = FColor::Blue;
			UpArrowComponent->bTreatAsASprite = true;
			UpArrowComponent->SetupAttachment(GizmoRootComponent);
			UpArrowComponent->bIsScreenSizeScaled = true;
			UpArrowComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

		}

		GizmoRootComponent->SetVisibility(true, true);
		GizmoRootComponent->SetHiddenInGame(true, true);
	}

	// Initialization

	CameraPitchMin = -89.0f;
	CameraPitchMax = 89.0f;

	RotationRate = 360.0f;
}

void ACustomPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!IsPendingKill())
	{
		if (PawnMesh)
		{

			// force animation tick after movement component updates
			if (PawnMesh->PrimaryComponentTick.bCanEverTick && MovementComponent)
			{
				PawnMesh->PrimaryComponentTick.AddPrerequisite(MovementComponent, MovementComponent->PrimaryComponentTick);
				SavePawnTransform(OldBaseQuat, OldBaseLocation);
			}
		}
	}

	CurrentForwardDirection = GetActorForwardVector();
	CurrentRightDirection = GetActorRightVector();
}

// Called every frame
void ACustomPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMeshRotation(DeltaTime);

	GizmoRootComponent->SetWorldRotation(FRotationMatrix::MakeFromXZ(CurrentForwardDirection, GetActorUpVector()).Rotator());

}

void ACustomPawn::Jump()
{
	if (MovementComponent == NULL) { return; }

	MovementComponent->DoJump();
}


void ACustomPawn::Sprint()
{
	if (MovementComponent == NULL) { return; }

	MovementComponent->DoSprint();
}

void ACustomPawn::StopSprint()
{
	if (MovementComponent == NULL) { return; }

	MovementComponent->DoStopSprint();
}

void ACustomPawn::AddForwardMovementInput(float ScaleValue /*= 1.0f*/, bool bForce /*= false*/)
{
	if (MovementComponent == NULL) { return; }

	const FVector UpDirection = GetActorUpVector();
	const FVector CameraForward = Camera->GetForwardVector();
	const float Dot = FVector::DotProduct(UpDirection, CameraForward);

	if (FMath::Abs(Dot) < 1 - SMALL_NUMBER)
	{
		CurrentForwardDirection = FVector::VectorPlaneProject(CameraForward, GetActorUpVector());
	}
	// Kevin VanHorn [3.18.19] This value is wrong because the mesh rotation is off from blender input
	/*if (bCameraForwardOverride) {
		CurrentForwardDirection = -1 * PawnMesh->GetRightVector();
	}*/

	const float ControlValue = MovementComponent->IsMovingOnGround() ? ScaleValue : ScaleValue * MovementComponent->AirControlRatio;
	AddMovementInput(CurrentForwardDirection.GetSafeNormal(), ControlValue, bForce);
}

void ACustomPawn::AddRightMovementInput(float ScaleValue /*= 1.0f*/, bool bForce /*= false*/)
{
	if (MovementComponent == NULL) { return; }

	const FVector UpDirection = GetActorUpVector();
	const FVector CameraRight = Camera->GetRightVector();
	const float Dot = FVector::DotProduct(UpDirection, CameraRight);

	if (FMath::Abs(Dot) < 1 - SMALL_NUMBER)
	{
		CurrentRightDirection = FVector::VectorPlaneProject(CameraRight, UpDirection);
	}

	// Kevin VanHorn [3.18.19] This value is wrong because the mesh rotation is off from blender input
	/*if (bCameraForwardOverride) {
		CurrentRightDirection = PawnMesh->GetForwardVector();
	}*/

	const float ControlValue = MovementComponent->IsMovingOnGround() ? ScaleValue : ScaleValue * MovementComponent->AirControlRatio;

	AddMovementInput(CurrentRightDirection.GetSafeNormal(), ControlValue, bForce);
}


void ACustomPawn::AddCameraPitchInput(float UpdateRate /*= 1.0f*/, float ScaleValue /*= 0.0f*/)
{
	if (SpringArm != NULL)
	{
		FRotator CameraRelativeRot = SpringArm->RelativeRotation;
		float CameraNewPitch = FMath::ClampAngle(CameraRelativeRot.Pitch + ScaleValue * UpdateRate, CameraPitchMin, CameraPitchMax);
		CameraRelativeRot.Pitch = CameraNewPitch;
		SpringArm->SetRelativeRotation(CameraRelativeRot);
	}
}

void ACustomPawn::AddPawnTurnInput(float UpdateRate, float ScaleValue)
{
	PawnMesh->AddRelativeRotation(FRotator(0.0f, ScaleValue * UpdateRate, 0.0f));
}

void ACustomPawn::AddCameraYawInput(float UpdateRate /*= 1.0f*/, float ScaleValue /*= 0.0f*/)
{
	if (SpringArm != NULL)
	{
		SpringArm->AddRelativeRotation(FRotator(0.0f, ScaleValue * UpdateRate, 0.0f));

		/* Update PawnMesh to change rotation with camera yaw (turning left-right): */
		//RotateMeshTo(FRotator(0.0f, ScaleValue * UpdateRate, 0.0f));
		//PawnMesh->AddRelativeRotation(FRotator(0.0f, ScaleValue * UpdateRate, 0.0f)); // Kevin VanHorn - [9.6.18]
	}
}

void ACustomPawn::EnableDebugging()
{
	if (CapsuleComponent != NULL) { CapsuleComponent->SetHiddenInGame(false); }
	if (GizmoRootComponent != NULL) { GizmoRootComponent->SetHiddenInGame(false, true); }
	if (MovementComponent != NULL) { MovementComponent->EnableDebuging(); }
}


void ACustomPawn::DisableDebugging()
{
	if (CapsuleComponent != NULL) { CapsuleComponent->SetHiddenInGame(true); }
	if (GizmoRootComponent != NULL) { GizmoRootComponent->SetHiddenInGame(true, true); }
	if (MovementComponent != NULL) { MovementComponent->DisableDebuging(); }
}

FVector ACustomPawn::GetCurrentForwardDirection() const
{
	return !CurrentForwardDirection.IsZero() ? CurrentForwardDirection : GetActorForwardVector();
}

FVector ACustomPawn::GetCurrentRightDirection() const
{
	return !CurrentRightDirection.IsZero() ? CurrentRightDirection : GetActorRightVector();
}


void ACustomPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (MovementComponent == NULL)
	{
		return;	
	}

	MovementComponent->CapsuleHited(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

void ACustomPawn::SavePawnTransform(FQuat& BaseQuat, FVector& BaseLoc)
{
	FTransform CurTransform = PawnMesh->GetComponentTransform();
	BaseQuat = CurTransform.GetRotation();
	BaseLoc = CurTransform.GetLocation();
}

void ACustomPawn::UpdateMeshRotation(float DeltaTime)
{
	//FRotator PawnOffset(0, 90, 0); // TODO: make this blueprint visible. 

	// Default Rotate Pawn to camera direction:
	//FRotator LocalRot = PawnMesh->RelativeRotation;
	//FRotator TargetRot(LocalRot.Pitch, SpringArm->RelativeRotation.Yaw, LocalRot.Roll);
	
	// Orient mesh rotation to movement direction override:
	//if (bOrientPawnRotationToMovement && MovementComponent) {
		//if (GetPendingMovementInputVector().SizeSquared() >= KINDA_SMALL_NUMBER)
		//{
			// Rotate toward direction of acceleration.
			//TargetRot = GetLastMovementInputVector().GetSafeNormal().Rotation();
		//}
	//}
	
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation()+ GetLastMovementInputVector()*100, FColor::Red, false, 2.0f);

	FVector UpAxis = GetActorUpVector();

	if (!bOrientPawnRotationToMovement) return;

	FRotator CurrentRotation = PawnMesh->GetComponentRotation(); // Normalized
	FRotator DeltaRot = GetDeltaRotation(DeltaTime); // Local Space

	// Convert DeltaRot to WorldSpace
	FRotator NewRot = CurrentForwardDirection.RotateAngleAxis(DeltaRot.Yaw, GetActorUpVector()).Rotation();
	DeltaRot = NewRot - CurrentRotation;

	FRotator DesiredRotation = CurrentRotation;
	
	DesiredRotation = GetLastMovementInputVector().GetSafeNormal().Rotation(); // World Space
	DesiredRotation.Normalize();

	// Accumulate a desired new rotation.
	const float AngleTolerance = 1e-3f;

	if (!CurrentRotation.Equals(DesiredRotation, AngleTolerance))
	{
		// PITCH
		if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, DesiredRotation.Pitch, AngleTolerance))
		{
			DesiredRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, DesiredRotation.Pitch, DeltaRot.Pitch);
		}

		// YAW
		if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, AngleTolerance))
		{
			DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRot.Yaw);
		}

		// ROLL
		if (!FMath::IsNearlyEqual(CurrentRotation.Roll, DesiredRotation.Roll, AngleTolerance))
		{
			DesiredRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, DesiredRotation.Roll, DeltaRot.Roll);
		}

		// Set the new rotation.
		//const FVector NewDelta = ConstrainDirectionToPlane(FVector::ZeroVector);
		PawnMesh->MoveComponent(FVector::ZeroVector, DesiredRotation, false);
		//PawnMesh->SetRelativeRotation(TargetRot);
	}

}

float ACustomPawn::GetAxisDeltaRotation(float InAxisRotationRate, float DeltaTime)
{
	return (InAxisRotationRate >= 0.f) ? (InAxisRotationRate * DeltaTime) : 360.f;
}

// Delta Yaw in local space.
FRotator ACustomPawn::GetDeltaRotation(float DeltaTime)
{
	return FRotator(0, GetAxisDeltaRotation(RotationRate, DeltaTime), 0);
}