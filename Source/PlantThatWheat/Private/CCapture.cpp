

#include "CCapture.h"
#include "Engine/Classes/Kismet/KismetRenderingLibrary.h"

// Sets default values
ACCapture::ACCapture()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Orientation = FRotator(-1,-1,-1); // Nonzero to trigger RT assignment events on quadrant change.
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
		Orientation = FRotator(90, 0, 0);
		Quadrant = EQuadrant::MBM; // Middle Ring: Middle Back
	}
	else if (PlayerRot.Pitch > 22.5 && PlayerRot.Pitch <= 67.5 && bIsPlayerBelow(PlayerRot.Yaw)) {
		Orientation = FRotator(135, 0, 0);
		Quadrant = EQuadrant::MBB; // Middle Ring: Bot Back
	}
	else if (PlayerRot.Pitch > -22.5 && PlayerRot.Pitch <= 22.5 && bIsPlayerBelow(PlayerRot.Yaw)) {
		Orientation = FRotator(180, 0, 0);
		Quadrant = EQuadrant::MBM; // Middle Ring: Bot // should be MBM
	}
	else if (PlayerRot.Pitch >= -67.5 && PlayerRot.Pitch <= -22.5 && bIsPlayerBelow(PlayerRot.Yaw)) {
		Orientation = FRotator(225, 0, 0);
		Quadrant = EQuadrant::MTB; // Middle Ring: Bot Front
	}
	else if (PlayerRot.Pitch < -67.5) {
		Orientation = FRotator(270, 0, 0);
		Quadrant = EQuadrant::MTM; // Middle Ring: Middle Front
	}
	else if (PlayerRot.Pitch >= -67.5 && PlayerRot.Pitch <= -22.5) {
		Orientation = FRotator(315, 0, 0);
		Quadrant = EQuadrant::MTT; // Middle Ring: Top Front
	}


	// Lateral Ring:
	if (PlayerRot.Roll < 22.5 && PlayerRot.Roll >= -22.5) {
		Orientation = FRotator::ZeroRotator;
		Quadrant = EQuadrant::MMT; // Lateral Ring: Top
		UE_LOG(LogTemp, Warning, TEXT("-------------------MMT"));
	}
	else if (PlayerRot.Roll < -22.5 && PlayerRot.Roll >= -67.5) {
		Orientation = Orientation = FRotator(0, 0, -45);
		Quadrant = EQuadrant::LMT; // Lateral Ring: Top Left
		UE_LOG(LogTemp, Warning, TEXT("-------------------LMT"));
	}
	else if (PlayerRot.Roll < -67.5 && PlayerRot.Roll >= -112.5){

		Orientation = Orientation = FRotator(0, 0, -90);
		Quadrant = EQuadrant::LMM; // Lateral Ring: Left Middle
		UE_LOG(LogTemp, Warning, TEXT("-------------------LMM"));
	}
	else if(PlayerRot.Roll < -112.5 && PlayerRot.Roll >= -157.5 && !bIsPlayerBelow(PlayerRot.Yaw)
		|| PlayerRot.Roll >= 112.5 && PlayerRot.Roll <= 157.5 && bIsPlayerBelow(PlayerRot.Yaw)){
		Orientation = FRotator(0, 0, -135);
		Quadrant = EQuadrant::LMB; // Lateral Ring: Left bot upper
		UE_LOG(LogTemp, Warning, TEXT("-------------------LMB"));
	}
	else if((PlayerRot.Roll > 157.5 && PlayerRot.Roll <= 180) ||
			(PlayerRot.Roll >= -180 && PlayerRot.Roll < -157.5)){
		Orientation = FRotator(180, 0, 0);
		Quadrant = EQuadrant::MMB; // Lateral Ring: Bottom most
		UE_LOG(LogTemp, Warning, TEXT("-------------------MMB"));
	}
	else if ((PlayerRot.Roll >= -157.5 && PlayerRot.Roll <= -112)
		||(PlayerRot.Roll <= 157.5 && PlayerRot.Roll >= 112.5)) {
		Orientation = FRotator(0, 0, -225);
		Quadrant = EQuadrant::RMB; // Lateral Ring: Bot Right
		UE_LOG(LogTemp, Warning, TEXT("-------------------RMB"));
	}
	else if ((PlayerRot.Roll < 112 && PlayerRot.Roll >= 67.5)){// ||
		//	(PlayerRot.Roll < 90 && PlayerRot.Roll > 67.5)) {
		Orientation = FRotator(0, 0,-270);
		Quadrant = EQuadrant::RMM; // Lateral Ring: Middle Right
		UE_LOG(LogTemp, Warning, TEXT("-------------------RMM"));
	}
	else if (PlayerRot.Roll < 67.5 && PlayerRot.Roll >=22.5) {
		Orientation = FRotator(0, 0, -315);
		Quadrant = EQuadrant::RMT; // Lateral Ring: Top Right
		UE_LOG(LogTemp, Warning, TEXT("-------------------RMT"));
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
}

// TODO: Optimize so getorthonormal base is not called twice.
FLinearColor ACCapture::GetOrthonormalBaseX(int32 index)   //0[0,1], 1[2,3], 2[4,5], 3[6,7], 4[8,9]
{
	if (index * 3 < OrthoBases.Num()) {
		return OrthoBases[index * 3];
	}
	return FLinearColor(0,0,0,0);
}

FLinearColor ACCapture::GetOrthonormalBaseY(int32 index)
{
	if ((index * 3 +1) < OrthoBases.Num()) {
		return OrthoBases[(index * 3) + 1];
	}
	return FLinearColor(0, 0, 0, 0);
}

FLinearColor ACCapture::GetOrthonormalBaseZ(int32 index)
{
	if ((index * 3 + 2) < OrthoBases.Num()) {
		return OrthoBases[(index * 3) + 2];
	}
	return FLinearColor(0, 0, 0, 0);
}

void ACCapture::PreCalcOrthoBases()
{
	FRotator before = GetActorRotation();

	for (int i = 0; i < RenderTargets.Num(); i++) {

		if (i == (int)EQuadrant::MMT) {
			direction = FRotator(0, 0, 0);
		}
		else if (i == (int)EQuadrant::MBT) {
			direction = FRotator(45, 0, 0);
		}
		else if (i == (int)EQuadrant::MBM) {
			direction = FRotator(90, 0, 0);
		}
		else if (i == (int)EQuadrant::MBB) {
			direction = FRotator(135, 0, 0);
		}
		else if (i == (int)EQuadrant::MBM) {
			direction = FRotator(180, 0, 0);
		}
		else if (i == (int)EQuadrant::MTB) {
			direction = FRotator(225, 0, 0);
		}
		else if (i == (int)EQuadrant::MTM) {
			direction = FRotator(270, 0, 0);
		}
		else if (i == (int)EQuadrant::MTT) {
			direction = FRotator(315, 0, 0);
		}

		// Lateral Ring
		else if (i == (int)EQuadrant::LMT) {
			direction = FRotator(0, 0, -45);
		}
		else if (i == (int)EQuadrant::LMM) {
			direction = FRotator(0, 0, -90);
		}
		else if (i == (int)EQuadrant::LMB) {
			direction = FRotator(0, 0, -135);
		}
		else if (i == (int)EQuadrant::MMB) {
			direction = FRotator(180, 0, 0);
		}
		else if (i == (int)EQuadrant::RMB) {
			direction = FRotator(0, 0, -225);
		}
		else if (i == (int)EQuadrant::RMM) {
			direction = FRotator(0, 0, -270);
		}
		else if (i == (int)EQuadrant::RMT) {
			direction = FRotator(0, 0, -315);
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

bool ACCapture::bIsPlayerBelow(float playerYaw)
{
	return FMath::IsNearlyEqual(playerYaw, 180, 0.01f) || FMath::IsNearlyEqual(playerYaw, -180, 0.01f);
}