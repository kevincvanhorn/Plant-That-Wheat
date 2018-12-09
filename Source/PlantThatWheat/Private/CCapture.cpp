

#include "CCapture.h"
#include "Engine/Classes/Kismet/KismetRenderingLibrary.h"

// Sets default values
ACCapture::ACCapture()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Orientation = FRotator::ZeroRotator;
	Quadrant = EQuadrant::MMT; // Middle Ring: Top
}

// Called when the game starts or when spawned
void ACCapture::BeginPlay()
{
	Super::BeginPlay();
	CreateRenderTargetArray();
	PreCalcOrthoBases();
}

// Called every frame
void ACCapture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACCapture::CreateOrthonormalBasis()
{
	FVector X = GetActorForwardVector();
	FVector Y = GetActorRightVector(); 
	FVector Z = GetActorUpVector();

	FVector::CreateOrthonormalBasis(X, Y, Z);

	OrthonormalX = X;
	OrthonormalY = Y;
	OrthonormalZ = Z;
}

bool ACCapture::SetCaptureOrientation(FRotator PlayerRot)
{
	FRotator Prev = Orientation;
	// Middle Ring:
	if (PlayerRot.Pitch > -22.5 && PlayerRot.Pitch <= 22.5 && FMath::IsNearlyZero(PlayerRot.Yaw, 0.01f)) {
		Orientation = FRotator::ZeroRotator;
		Quadrant = EQuadrant::MMT; // Middle Ring: Top
	}
	else if (PlayerRot.Pitch > 22.5 && PlayerRot.Pitch <= 67.5 && PlayerRot.Yaw == 0) {
		Orientation = FRotator(45, 0, 0);
		Quadrant = EQuadrant::MBT; // Middle Ring: Top Back
	}
	else if (PlayerRot.Pitch > 67.5) {
		Orientation = FRotator::ZeroRotator;
		Quadrant = EQuadrant::MBM; // Middle Ring: Middle Back
	}
	else if (PlayerRot.Pitch > 22.5 && PlayerRot.Pitch <= 67.5 && PlayerRot.Yaw == 180) {
		Orientation = FRotator(90, 0, 0);
		Quadrant = EQuadrant::MBB; // Middle Ring: Bot Back
	}
	else if (PlayerRot.Pitch > -22.5 && PlayerRot.Pitch <= 22.5 && PlayerRot.Yaw == 180) {
		Orientation = FRotator(135, 0, 0);
		Quadrant = EQuadrant::MMB; // Middle Ring: Bot
	}
	else if (PlayerRot.Pitch >= -67.5 && PlayerRot.Pitch <= -22.5 && PlayerRot.Yaw == 180) {
		Orientation = FRotator(180, 0, 0);
		Quadrant = EQuadrant::MTB; // Middle Ring: Bot Front
	}
	else if (PlayerRot.Pitch < -67.5) {
		Orientation = FRotator(225, 0, 0);
		Quadrant = EQuadrant::MTM; // Middle Ring: Middle Front
	}
	else if (PlayerRot.Pitch >= -67.5 && PlayerRot.Pitch <= -22.5) {
		Orientation = FRotator(270, 0, 0);
		Quadrant = EQuadrant::MTT; // Middle Ring: Top Front
	}
	
	if (Orientation == Prev)
		return false;
	return true;
}

UTextureRenderTarget2D * ACCapture::GetRenderTargetByIndex(int32 RT_Index)
{
	if (RT_Index < RenderTargets.Num()) {
		return RenderTargets[RT_Index];
	}
	return nullptr;
}

void ACCapture::CreateRenderTargetArray()
{
	for (int i = 0; i < NUM_RT; i++) {
		UTextureRenderTarget2D* RT = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), RT_WIDTH, RT_HEIGHT, ETextureRenderTargetFormat::RTF_RGBA16f);
		RenderTargets.Emplace(RT);
	}
	;
}

// TODO: Optimize so getorthonormal base is not called twice.
FLinearColor ACCapture::GetOrthonormalBaseX(int32 index)   //0[0,1], 1[2,3], 2[4,5], 3[6,7], 4[8,9]
{
	if (index * 2 < OrthoBases.Num()) {
		return OrthoBases[index * 2];
	}
	return FLinearColor(0,0,0,0);
}

FLinearColor ACCapture::GetOrthonormalBaseY(int32 index)
{
	if ((index * 2 +1) < OrthoBases.Num()) {
		return OrthoBases[(index * 2) + 1];
	}
	return FLinearColor(0, 0, 0, 0);
}

void ACCapture::PreCalcOrthoBases()
{
	FRotator before = GetActorRotation();

	for (int i = 0; i < RenderTargets.Num(); i++) {

		if (i == (int)EQuadrant::MMT) {
			direction = FRotator(0,0,0);
		}
		else if (i == (int)EQuadrant::MBT) {
			direction = FRotator(45, 0, 0);
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
	}

	SetActorRotation(before);
}
