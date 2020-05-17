// Copyright 2015 Elhoussine Mehnik (Mhousse1247). All Rights Reserved.
//******************* http://ue4resources.com/ *********************//



#include "GravityCharacter.h"
#include "CustomGravityPluginPrivatePCH.h"
#include "GameFramework/SpringArmComponent.h"

AGravityCharacter::AGravityCharacter()
{

	GetSpringArm()->TargetArmLength = 600.0f;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	MeshOrientation = EMeshOrientation::EMO_Movement;
	bRotateMeshOnlyWhenMoving = true;
	MinVelocityToRotateMesh = 2.0f;
	bInstantRotation = true;
	RotationInterpSpeed = 5.0f;
}

// Called when the game starts or when spawned
void AGravityCharacter::BeginPlay()
{
	Super::BeginPlay();

	MeshStartRotation = GetMesh()->GetRelativeRotation();
}


void AGravityCharacter::UpdateMeshRotation(float DeltaTime)
{
	
	const float Speed = GetMovementComponent() != NULL ? GetMovementComponent()->Velocity.Size() : 0.0f;

	if (bRotateMeshOnlyWhenMoving && Speed < MinVelocityToRotateMesh)
	{
		return;
	}

	FRotator MeshRotation = GetMesh()->GetRelativeRotation();
	const FVector ProjectedVelocity = FVector::VectorPlaneProject(GetMovementComponent()->Velocity, GetActorUpVector());
	const FRotator Rot = FRotationMatrix::MakeFromXZ(GetTransform().InverseTransformVector(ProjectedVelocity), GetActorUpVector()).Rotator();

	MeshRotation.Yaw = MeshOrientation == EMeshOrientation::EMO_Movement ?
		MeshStartRotation.Yaw + Rot.Yaw :
		MeshStartRotation.Yaw + GetSpringArm()->GetRelativeRotation().Yaw;

	FRotator MRot = bInstantRotation ? MeshRotation : FMath::RInterpTo(GetMesh()->GetRelativeRotation(), MeshRotation, DeltaTime, RotationInterpSpeed);
	GetMesh()->SetRelativeRotation(MRot);
}



