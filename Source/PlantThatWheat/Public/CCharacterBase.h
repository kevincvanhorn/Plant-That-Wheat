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

UENUM(BlueprintType)
enum class EToolMode : uint8 {
		Default,
		Shovel,
		Planting,
		Harvest
};

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

	// Uses camera position as the Eye location instead of root + eyeheight for use in Multitool 
	//virtual FVector GetPawnViewLocation() const override; // Replace Pawn implementation to camera view.

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	void _AddCameraPitchInput(float Val);

	void _AddCameraYawInput(float Val);

/* Modes and Multitool: */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TEnumAsByte<EToolMode> ToolMode;

/* Usable Actor: https://www.tomlooman.com/tutorial-usableactor-system-in-c/ */
protected:
	/** Get UsableActor based actor that the character is looking at. */
	//class AUsableActor* TraceForUsableActor();

	/* Max distance to use/focus on actors. */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	//	float MaxUseDistance;

	/* True once each new focus of a usable actor (first frame in focus). */
	//bool bUsableHasNewFocus;

	/* Actor derived from UsableActor currently in center-view. */
	//AUsableActor* UsableActorInFocus;

public:

	/** Use the actor currently in view (if derived from UsableActor) */
	//UFUNCTION(BlueprintCallable, WithValidation, Server, Reliable, Category = "Gameplay")
	//	virtual void UseUsableActor();
};
