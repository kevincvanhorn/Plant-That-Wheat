// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomGravity/CustomPawn.h"
#include "CCharacterBase.generated.h"

class UCameraComponent; // Forward Declartion.
class USpringArmComponent;
class ACMultiTool;
class ACPickupActor;
class ACGameMode;
class ACPlayerState;
class ACPlantingTool;
class ACGunTool;
class ACShovelTool;
class ACDefaultTool;
class ACHarvestTool;

UENUM(BlueprintType)
enum class EToolMode : uint8 {	
		Default,
		Shovel,
		Planting,
		Harvest,
		Weapon,
		_Last
};

UCLASS()
class PLANTTHATWHEAT_API ACCharacterBase : public ACustomPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACCharacterBase();

	ACMultiTool* CurrentTool;
	ACGunTool* WeaponTool;
	ACDefaultTool* DefaultTool;
	ACShovelTool* ShovelTool;
	ACPlantingTool* PlantingTool;
	ACHarvestTool* HarvestTool;

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

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		TSubclassOf<ACGunTool> WeaponToolClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		TSubclassOf<ACDefaultTool> DefaultToolClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		TSubclassOf<ACShovelTool> ShovelToolClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		TSubclassOf<ACPlantingTool> PlantingToolClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		TSubclassOf<ACHarvestTool> HarvestToolClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Player")
		FName ToolAttachSocketName;

	void Fire();

	void SwitchToolMode(EToolMode NewToolMode);

	void SwitchTool();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Uses camera position as the Eye location instead of root + eyeheight for use in Multitool 
	virtual FVector GetPawnViewLocation() const override; // Replace Pawn implementation to camera view.

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	void _AddCameraPitchInput(float Val);

	void _AddCameraYawInput(float Val);

/* Modes and Multitool: */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		EToolMode ToolMode;

private:
	uint8 CurToolModeIndex;
	TArray<EToolMode> ActiveTools = {EToolMode::Default, EToolMode::Shovel, EToolMode::Planting, EToolMode::Harvest};

/* Pickups: */
public:
	virtual void OnPickupItem(ACPickupActor* Pickup);

protected:
	ACGameMode* GameMode;

	ACPlayerState* CPlayerState;
};
