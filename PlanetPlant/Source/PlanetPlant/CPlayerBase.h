// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CPlayerBase.generated.h" // [Must be last] Part of Unreal's Reflection System (along w/ UCLASS & GENERATED_BODY)

UCLASS()
class PLANETPLANT_API ACPlayerBase : public APawn
{
	GENERATED_BODY()

	/** StaticMesh to render for the pawn. */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh; // UE4: "class" Forward Declaration to minimize includes in header.

	/** Spring arm to offset camera (camera boom). */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Viewport camera component. */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

public:
	ACPlayerBase();

public:
	// Begin AActor overrides:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End AActor overrides.

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; // Binds functionality to input.
	
	class UCPlayerBase_MovementComponent* CustomMovementComponent;
	virtual UPawnMovementComponent* GetMovementComponent() const override;

protected:
	/* Movement Functions: */
	void MoveForwardInput(float AxisValue);
	void MoveLateralInput(float AxisValue);
	void TurnInput(float AxisValue);

private:
	/** Forward acceleration of Pawn. */
	UPROPERTY(Category = Movement, EditAnywhere)
		float MoveSpeed;

/* Accessors: */
public:
	/** Returns PlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetP1Mesh() const { return Mesh; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
};
