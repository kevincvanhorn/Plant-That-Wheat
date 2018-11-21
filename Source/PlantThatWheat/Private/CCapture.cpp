

#include "CCapture.h"


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
	if (PlayerRot.Pitch >= 0 && PlayerRot.Pitch <= 30) {
		Orientation = FRotator::ZeroRotator;
	}
	else if (PlayerRot.Pitch > 30 && PlayerRot.Pitch <= 90) {
		Orientation = FRotator(60, 0, 0);
	}
	if (Orientation == Prev)
		return false;
	return true;
}

