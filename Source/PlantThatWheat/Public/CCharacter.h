// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CCharacter.generated.h"

class UCameraComponent; // Forward Declartion.
class USpringArmComponent;
class ACMultiTool;

UCLASS()
class PLANTTHATWHEAT_API ACCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void BeginCrouch();

	void EndCrouch();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") // Can only edit properties is BP, not pass in new object.
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	bool bWantsToZoom;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomInterpSpeed; 

	/* Default FOV set during BeginPlay. */
	float DefaultFOV;

	void BeginZoom();

	void EndZoom();

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
