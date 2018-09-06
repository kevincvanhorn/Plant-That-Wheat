// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomGravity/CustomPawn.h"
#include "CCharacterBase.generated.h"

/**
 * 
 */
class UCameraComponent; // Forward Declartion.
class USpringArmComponent;
class ACMultiTool;

UCLASS()
class PLANTTHATWHEAT_API ACCharacterBase : public ACustomPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	//void BeginCrouch();

	//void EndCrouch();

	bool bWantsToZoom;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
		float ZoomInterpSpeed;

	/* Default FOV set during BeginPlay. */
	float DefaultFOV;

	void BeginZoom();

	void EndZoom();

	/* Tool Functions: */

	ACMultiTool* CurrentTool;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		TSubclassOf<ACMultiTool> StarterToolClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Player")
		FName ToolAttachSocketName;

	void Fire();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override; // Replace Pawn implementation to camera view.

};
