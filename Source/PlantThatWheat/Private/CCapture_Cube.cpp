

#include "CCapture_Cube.h"
#include "Engine/Classes/Kismet/KismetRenderingLibrary.h"

// Sets default values
ACCapture_Cube::ACCapture_Cube()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Orientation = FRotator(-1, -1, -1); // Nonzero to trigger RT assignment events on quadrant change.
	Quadrant = EQuadrant_Cube::TOP; // Middle Ring: Top
}

// Called when the game starts or when spawned
void ACCapture_Cube::BeginPlay()
{
	Super::BeginPlay();
	CreateRenderTargetArray();
	PreCalcOrthoBases();
}

// Called every frame
void ACCapture_Cube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACCapture_Cube::CreateOrthonormalBasis()
{
	FVector X = GetActorForwardVector();
	FVector Y = GetActorRightVector();
	FVector Z = GetActorUpVector();

	FVector::CreateOrthonormalBasis(X, Y, Z);

	OrthonormalX = X;
	OrthonormalY = Y;
	OrthonormalZ = Z;
}

bool ACCapture_Cube::SetCaptureOrientation(FRotator PlayerRot)
{
	FRotator Prev = Orientation;
	SetQuadrant(PlayerRot);
	if (Orientation == Prev)
		return false;
	return true;
}


UTextureRenderTarget2D * ACCapture_Cube::GetRenderTargetByIndex(int32 RT_Index)
{
	if (RT_Index < RenderTargets.Num()) {
		return RenderTargets[RT_Index];
	}
	return nullptr;
}

void ACCapture_Cube::CreateRenderTargetArray()
{
	for (int i = 0; i < NUM_RT; i++) {
		UTextureRenderTarget2D* RT = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), RT_WIDTH, RT_HEIGHT, ETextureRenderTargetFormat::RTF_RGBA16f);
		RenderTargets.Emplace(RT);
	}
}

// TODO: Optimize so getorthonormal base is not called twice.
FLinearColor ACCapture_Cube::GetOrthonormalBaseX(int32 index)   //0[0,1], 1[2,3], 2[4,5], 3[6,7], 4[8,9]
{
	if (index * 3 < OrthoBases.Num()) {
		return OrthoBases[index * 3];
	}
	return FLinearColor(0, 0, 0, 0);
}

FLinearColor ACCapture_Cube::GetOrthonormalBaseY(int32 index)
{
	if ((index * 3 + 1) < OrthoBases.Num()) {
		return OrthoBases[(index * 3) + 1];
	}
	return FLinearColor(0, 0, 0, 0);
}

FLinearColor ACCapture_Cube::GetOrthonormalBaseZ(int32 index)
{
	if ((index * 3 + 2) < OrthoBases.Num()) {
		return OrthoBases[(index * 3) + 2];
	}
	return FLinearColor(0, 0, 0, 0);
}

void ACCapture_Cube::PreCalcOrthoBases()
{
	FRotator before = GetActorRotation();

	for (uint8 i = 0; i < RenderTargets.Num(); i++) {

		//SetOrientation((EQuadrant)i, false);

		if (i == (int)EQuadrant_Cube::TOP) {
			direction = FRotator(0, 0, 0);
		}
		else if (i == (int)EQuadrant_Cube::FORWARD) { // MBM
			direction = FRotator(90, 0, 0);
		}
		else if (i == (int)EQuadrant_Cube::BOT) { // MMB
			direction = FRotator(180, 0, 0);
		}
		else if (i == (int)EQuadrant_Cube::BACK) { // MTM
			direction = FRotator(270, 0, 0);
		}
		// Lateral Ring
		else if (i == (int)EQuadrant_Cube::LEFT) { //LMM
			direction = FRotator(90, 90, 0);
		}
		else if (i == (int)EQuadrant_Cube::RIGHT) { //RMM
			direction = FRotator(270, 90, 0);
		}

		else {
			direction = FRotator::ZeroRotator;
		}

		SetActorRotation(direction);

		FVector X = GetActorForwardVector();
		FVector Y = GetActorRightVector();
		FVector Z = GetActorUpVector();

		FVector::CreateOrthonormalBasis(X, Y, Z);

		OrthoBases.Emplace(X.X, X.Y, X.Z, 1);
		OrthoBases.Emplace(Y.X, Y.Y, Y.Z, 1);
		OrthoBases.Emplace(Z.X, Z.Y, Z.Z, 1);
	}

	SetActorRotation(before);
}

bool ACCapture_Cube::bIsPlayerBelow(float playerYaw)
{
	return FMath::IsNearlyEqual(playerYaw, 180, 0.5f) || FMath::IsNearlyEqual(playerYaw, -180, 0.5f);
}

bool ACCapture_Cube::bIsWithinQuadrant(const EQuadrant_Cube &quad, const FRotator &PlayerRot)
{
	return false;
}

void ACCapture_Cube::SetQuadrant(const FRotator &PlayerRot) {
	//float L = PlayerRot.Pitch - 22.5; // Length from top of triangle.

	// Top Half:   ----------------------------
	if (FMath::IsNearlyZero(PlayerRot.Yaw, 0.5f)) {
		// Forward Ring:
		if (FMath::Abs(PlayerRot.Roll) <= 45) {
			if (FMath::Abs(PlayerRot.Pitch) <= 45) { SetOrientation(EQuadrant_Cube::TOP, true); }
			else if (PlayerRot.Pitch > 45 && PlayerRot.Pitch <= 90) { SetOrientation(EQuadrant_Cube::FORWARD, true); }
			else if (PlayerRot.Pitch <= -45 && PlayerRot.Pitch > -90) { SetOrientation(EQuadrant_Cube::BACK, true); }
		}
		// Lateral Ring:
		else {
			if (PlayerRot.Roll <= -45 && PlayerRot.Roll > -90) { SetOrientation(EQuadrant_Cube::LEFT, true); }
			else if (PlayerRot.Roll > 45 && PlayerRot.Roll <= 90) { SetOrientation(EQuadrant_Cube::RIGHT, true); }
		}
	}

	// Bottom Half: ---------------------------
	else if (bIsPlayerBelow(PlayerRot.Yaw)) {
		// Forward Ring:
		if (FMath::Abs(PlayerRot.Roll) >= 135) {
			if(FMath::Abs(PlayerRot.Pitch) <= 45) { SetOrientation(EQuadrant_Cube::BOT, true); }
			else if (PlayerRot.Pitch > 45 && PlayerRot.Pitch <= 90) { SetOrientation(EQuadrant_Cube::FORWARD, true); }
			else if (PlayerRot.Pitch <= -45 && PlayerRot.Pitch > -90) { SetOrientation(EQuadrant_Cube::BACK, true); }
		}
		else {
			if (PlayerRot.Roll > 90 && PlayerRot.Roll <= 180) { SetOrientation(EQuadrant_Cube::LEFT, true); }
			else if (PlayerRot.Roll <= -90 && PlayerRot.Roll > -180) { SetOrientation(EQuadrant_Cube::RIGHT, true); }
		}
	}
}

void ACCapture_Cube::SetOrientation(EQuadrant_Cube quad, bool bSetQuadrant)
{
	if (bSetQuadrant) { Quadrant = quad; }

	if (quad == EQuadrant_Cube::TOP) {
		Orientation = FRotator(0, 0, 0);
		UE_LOG(LogTemp, Warning, TEXT("-------------------TOP"));
	}
	else if (quad == EQuadrant_Cube::FORWARD) { // MBM
		Orientation = FRotator(90, 0, 0);
		UE_LOG(LogTemp, Warning, TEXT("-------------------FORWARD"));

	}
	else if (quad == EQuadrant_Cube::BOT) { // MMB
		Orientation = FRotator(180, 0, 0);
		UE_LOG(LogTemp, Warning, TEXT("-------------------BOT"));

	}
	else if (quad == EQuadrant_Cube::BACK) { // MTM
		Orientation = FRotator(270, 0, 0);
		UE_LOG(LogTemp, Warning, TEXT("-------------------BACK"));
	}
	// Lateral Ring
	else if (quad == EQuadrant_Cube::LEFT) { //LMM
		Orientation = FRotator(90, 90, 0);
		UE_LOG(LogTemp, Warning, TEXT("-------------------LEFT"));
	}
	else if (quad == EQuadrant_Cube::RIGHT) { //RMM
		Orientation = FRotator(270, 90, 0);
		UE_LOG(LogTemp, Warning, TEXT("-------------------RIGHT"));
	}

	else {
		Orientation = FRotator::ZeroRotator;
	}
}


