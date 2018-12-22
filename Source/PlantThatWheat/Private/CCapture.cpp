

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

	SetQuadrant(PlayerRot);
	// Middle Ring:
	/*else if ((PlayerRot.Roll < -22.5)) {
		UE_LOG(LogTemp, Warning, TEXT("MID-------------------LTM"));
		Quadrant = EQuadrant::LTM; // Middle Ring : Top left
		//Orientation = FRotator(225, -45, 0);
	}
	else if ((PlayerRot.Roll > 22.5)) {
		UE_LOG(LogTemp, Warning, TEXT("MID-------------------RTM"));
		Quadrant = EQuadrant::RTM; // Middle Ring: Top Right
		//Orientation = FRotator(225, -315, 0);
	}
	else if (true) {
		UE_LOG(LogTemp, Warning, TEXT("MID-------------------RBM"));
		Quadrant = EQuadrant::RBM; // Middle Ring: Bottom Right
	}
	else if (true) {
		UE_LOG(LogTemp, Warning, TEXT("MID-------------------RTM"));
		Quadrant = EQuadrant::LBM; // Middle Ring: Bottom Left 
	}*/

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

	for (uint8 i = 0; i < RenderTargets.Num(); i++) {
		
		//SetOrientation((EQuadrant)i, false);

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
		else if (i == (int)EQuadrant::MMB) {
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
			direction = FRotator(45, 90, 0);
		}
		else if (i == (int)EQuadrant::LMM) {
			direction = FRotator(90, 90, 0);
		}
		else if (i == (int)EQuadrant::LMB) {
			direction = FRotator(135, 90, 0);
		}
		else if (i == (int)EQuadrant::RMB) {
			direction = FRotator(225, 90, 0);
		}
		else if (i == (int)EQuadrant::RMM) {
			direction = FRotator(270, 90, 0);
		}
		else if (i == (int)EQuadrant::RMT) {
			direction = FRotator(315, 90, 0);
		}

		// Top Corners
		else if (i == (int)EQuadrant::LBT) {
			direction = FRotator(45,45,0);
		}
		else if (i == (int)EQuadrant::LTT) {
			direction = FRotator(45, 135, 0);
		}
		else if (i == (int)EQuadrant::RTT) {
			direction = FRotator(45, 225, 0);
		}
		else if (i == (int)EQuadrant::RBT) {
			direction = FRotator(45, 315, 0);
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

bool ACCapture::bIsWithinQuadrant(const EQuadrant &quad, const FRotator &PlayerRot)
{
	return false;
}

void ACCapture::SetQuadrant(const FRotator &PlayerRot) {
	float L = PlayerRot.Pitch - 22.5; // Length from top of triangle.

	// Top Half: Forward ring: ----------------------------
	if (FMath::IsNearlyZero(PlayerRot.Yaw, 0.1f)) {
		if (FMath::Abs(PlayerRot.Roll) <= 22.5f) {
			if (FMath::Abs(PlayerRot.Pitch) <= 22.5f) { SetOrientation(EQuadrant::MMT, true); }
			else if (PlayerRot.Pitch > 22.5 && PlayerRot.Pitch <= 67.5) { SetOrientation(EQuadrant::MBT, true); }
			else if (PlayerRot.Pitch > 67.5) { SetOrientation(EQuadrant::MBM, true); }
			else if (PlayerRot.Pitch < -67.5) { SetOrientation(EQuadrant::MTM, true); }
			else if (PlayerRot.Pitch >= -67.5 && PlayerRot.Pitch <= -22.5) { SetOrientation(EQuadrant::MTT, true); }
		}

		// Lateral Ring Transition: 
		else if (FMath::Abs(PlayerRot.Pitch) <= 22.5) {
			if (PlayerRot.Roll <= -22.5 && PlayerRot.Roll >= -67.5) { SetOrientation(EQuadrant::LMT, true); }
			else if (PlayerRot.Roll < 67.5f && PlayerRot.Roll >= 22.5f) { SetOrientation(EQuadrant::RMT, true); }
		}

		// Top Ring Transition:
		else if (PlayerRot.Pitch > 22.5 && PlayerRot.Pitch <= 67.5) {
			if (PlayerRot.Roll < -22.5 && PlayerRot.Roll >= -67.5) { SetOrientation(EQuadrant::LBT, true); }
		}
		else if (PlayerRot.Pitch < -22.5 && PlayerRot.Pitch >= -67.5f) {
			if (PlayerRot.Roll < -22.5 && PlayerRot.Roll >= -67.5) { SetOrientation(EQuadrant::LTT, true); }
		}
	}

	// Ring 45:
	else if (FMath::IsNearlyEqual(PlayerRot.Yaw, 45, .01f)) {
		if (PlayerRot.Pitch > 22.5 && PlayerRot.Pitch <= 67.5) {
			if (FMath::Abs(PlayerRot.Roll) <= L) { SetOrientation(EQuadrant::LBT, true); }
			else if (PlayerRot.Roll > L) { SetOrientation(EQuadrant::MBT, true); }
			else if (PlayerRot.Roll < L) { SetOrientation(EQuadrant::LMT, true); }
		}
	}

	// Ring 135:
	else if (FMath::IsNearlyEqual(PlayerRot.Yaw, 135, .01f)) {
		if (PlayerRot.Pitch > 22.5 && PlayerRot.Pitch <= 67.5) {
			if (FMath::Abs(PlayerRot.Roll) <= L) { SetOrientation(EQuadrant::LTT, true); }
			else if (PlayerRot.Roll > L) { SetOrientation(EQuadrant::LMT, true); }
			else if (PlayerRot.Roll < L) { SetOrientation(EQuadrant::MTT, true); }
		}
	}

	// Lateral Ring: -------------------------------------
	else if (FMath::IsNearlyEqual(PlayerRot.Yaw, 90, .01f)) {
		if (FMath::Abs(PlayerRot.Roll) <= 22.5f) {
			if (PlayerRot.Pitch >= 22.5f && PlayerRot.Pitch <= 67.5) { SetOrientation(EQuadrant::LMT, true); }
			else if(PlayerRot.Pitch > 67.5 && PlayerRot.Pitch <= 90) { SetOrientation(EQuadrant::LMM, true); }
			else if (PlayerRot.Pitch >= -90 && PlayerRot.Pitch <= -67.5) { SetOrientation(EQuadrant::RMM, true); }
			else if (PlayerRot.Pitch > -67.5 && PlayerRot.Pitch <= -22.5) { SetOrientation(EQuadrant::RMT, true); }
			
			// Forward Ring Transition:
			else if (FMath::Abs(PlayerRot.Pitch) < 22.5f) { SetOrientation(EQuadrant::MMT, true); }
		}
		// Top Ring Transition:
		else if (PlayerRot.Roll > 22.5f) { SetOrientation(EQuadrant::LBT, true); }
		else if(PlayerRot.Roll < -22.5f) { SetOrientation(EQuadrant::LTT, true); }
	}
	else if (FMath::IsNearlyEqual(PlayerRot.Yaw, -90, .01f)) {
		if (PlayerRot.Pitch > 67.5 && PlayerRot.Pitch <= 90) { SetOrientation(EQuadrant::LMM, true); }
		else if (PlayerRot.Pitch >= 22.5f && PlayerRot.Pitch <= 67.5) { SetOrientation(EQuadrant::LMB, true); }
		else if (PlayerRot.Pitch <= -22.5f && PlayerRot.Pitch >= -67.5f) { SetOrientation(EQuadrant::RMB, true); }
		else if (PlayerRot.Pitch < -67.5f && PlayerRot.Pitch >= -90) { SetOrientation(EQuadrant::RMM, true); }
		
		// Forward Ring Transition:
		else if (PlayerRot.Pitch < 22.45f && PlayerRot.Pitch >= 0) { SetOrientation(EQuadrant::MMB, true); }
	}
	// Bottom Half: ---------------------------
	else if(bIsPlayerBelow(PlayerRot.Yaw)) {
		// Forward Ring:
		if(FMath::Abs(PlayerRot.Roll) >= 157.5){
			if(PlayerRot.Pitch > 67.5) { SetOrientation(EQuadrant::MBM, true); }
			else if(PlayerRot.Pitch >= 22.5 && PlayerRot.Pitch <= 67.5) { SetOrientation(EQuadrant::MBB, true); }
			else if(FMath::Abs(PlayerRot.Pitch) <= 22.5) { SetOrientation(EQuadrant::MMB, true); } // Bottom
			else if(PlayerRot.Pitch >= -67.5 && PlayerRot.Pitch <= -22.5) { SetOrientation(EQuadrant::MTB, true); }
			else if(PlayerRot.Pitch < -67.5) { SetOrientation(EQuadrant::MTM, true); }
		}
		// Lateral Ring:
		else if (PlayerRot.Roll < 157.5f && PlayerRot.Roll >= 112.5f) { SetOrientation(EQuadrant::LMM, true); }
		else if (PlayerRot.Roll >= -157.5f && PlayerRot.Roll <= -112.5f) { SetOrientation(EQuadrant::RMB, true); }
	}
}

void ACCapture::SetOrientation(EQuadrant quad, bool bSetQuadrant)
{
	if (bSetQuadrant) { Quadrant = quad; }

	if (quad == EQuadrant::MMT) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------MMT"));
		Orientation = FRotator(0, 0, 0);
	}
	else if (quad == EQuadrant::MBT) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------MBT"));
		Orientation = FRotator(45, 0, 0);
	}
	else if (quad == EQuadrant::MBM) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------MBM"));
		Orientation = FRotator(90, 0, 0);
	}
	else if (quad == EQuadrant::MBB) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------MBB"));
		Orientation = FRotator(135, 0, 0);
	}
	else if (quad == EQuadrant::MMB) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------MMB"));
		Orientation = FRotator(180, 0, 0);
	}
	else if (quad == EQuadrant::MTB) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------MTB"));
		Orientation = FRotator(225, 0, 0);
	}
	else if (quad == EQuadrant::MTM) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------MTM"));
		Orientation = FRotator(270, 0, 0);
	}
	else if (quad == EQuadrant::MTT) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------MTT"));
		Orientation = FRotator(315, 0, 0);
	}

	// Lateral Ring
	else if (quad == EQuadrant::LMT) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------LMT"));
		Orientation = FRotator(45, 90, 0);
	}
	else if (quad == EQuadrant::LMM) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------LMM"));
		Orientation = FRotator(90, 90, 0);
	}
	else if (quad == EQuadrant::LMB) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------LMB"));
		Orientation = FRotator(135, 90, 0);
	}
	else if (quad == EQuadrant::RMB) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------RMB"));
		Orientation = FRotator(225, 90, 0);
	}
	else if (quad == EQuadrant::RMM) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------RMM"));
		Orientation = FRotator(270, 90, 0);
	}
	else if(quad == EQuadrant::RMT) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------RMT"));
		Orientation = FRotator(315, 90, 0);
	}

	// Top Corners
	else if (quad == EQuadrant::LBT) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------LBT"));
		Orientation = FRotator(45, 45, 0);
	}
	else if (quad == EQuadrant::LTT) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------LTT"));
		Orientation = FRotator(45, 135, 0);
	}
	else if (quad == EQuadrant::RTT) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------RTT"));
		Orientation = FRotator(45, 225, 0);
	}
	else if (quad == EQuadrant::RBT) {
		UE_LOG(LogTemp, Warning, TEXT("-------------------RBT"));
		Orientation = FRotator(45, 315, 0);
	}

	else {
		Orientation = FRotator::ZeroRotator;
	}
}
