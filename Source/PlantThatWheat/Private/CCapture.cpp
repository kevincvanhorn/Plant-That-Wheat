

#include "CCapture.h"
#include "Engine/Classes/Kismet/KismetRenderingLibrary.h"

// Sets default values
ACCapture::ACCapture()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Orientation = FRotator::ZeroRotator;
}

// Called when the game starts or when spawned
void ACCapture::BeginPlay()
{
	Super::BeginPlay();
	
	CreateRenderTargetArray();
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
	if (PlayerRot.Pitch > -22.5 && PlayerRot.Pitch <= 22.5 && PlayerRot.Roll == 0) {
		Orientation = FRotator::ZeroRotator;
		Quadrant = EQuadrant::MMT; // Middle Ring: Top
	}
	else if (PlayerRot.Pitch > 22.5 && PlayerRot.Pitch <= 67.5 && PlayerRot.Roll == 0) {
		Orientation = FRotator(45, 0, 0);
		Quadrant = EQuadrant::MBT; // Middle Ring: Top Back
	}
	else if (PlayerRot.Pitch > 67.5) {
		Orientation = FRotator::ZeroRotator;
		Quadrant = EQuadrant::MBM; // Middle Ring: Middle Back
	}
	else if (PlayerRot.Pitch > 22.5 && PlayerRot.Pitch <= 67.5 && PlayerRot.Roll == 180) {
		Orientation = FRotator(90, 0, 0);
		Quadrant = EQuadrant::MBB; // Middle Ring: Bot Back
	}
	else if (PlayerRot.Pitch > -22.5 && PlayerRot.Pitch <= 22.5 && PlayerRot.Roll == 180) {
		Orientation = FRotator(135, 0, 0);
		Quadrant = EQuadrant::MMB; // Middle Ring: Bot
	}
	else if (PlayerRot.Pitch >= -67.5 && PlayerRot.Pitch <= -22.5 && PlayerRot.Roll == 180) {
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
	RenderTargets.Emplace(UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), RT_WIDTH, RT_HEIGHT, ETextureRenderTargetFormat::RTF_RGBA16f));
	RenderTargets.Emplace(UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), RT_WIDTH, RT_HEIGHT, ETextureRenderTargetFormat::RTF_RGBA16f));
}

