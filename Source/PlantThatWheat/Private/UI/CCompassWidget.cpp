

#include "CCompassWidget.h"
#include "CGameMode.h"
#include "CCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"

/*void UCCompassWidget::Init(const ACGameMode* GameMode, const ACCharacter* Character) {
	this->GameMode = GameMode;
	this->Character = Character;
	GetCharacterRotation();
}*/

void UCCompassWidget::Init(ACGameMode* GameMode, ACCharacter* Character) {
	this->GameMode = GameMode;
	this->Character = Character;
}

float UCCompassWidget::GetRotationOffset()
{
	if (Character) {

		// Find the angle to orient toward the pole (north):
		FVector VActorForward = Character->GetForwardArrowComponent()->GetForwardVector();
		FVector TopPoint = FVector(0, 0, 0) + FVector(0, 0, 1587); // TODO: Use planet center and radius.
		FVector VToPole = TopPoint - Character->GetActorLocation();
		
		// Project onto the character's plane by subtracting off the component of VToPole that is orthoganal to the character plane.
		VToPole = FVector::VectorPlaneProject(VToPole, Character->GetForwardArrowComponent()->GetUpVector());

		float AngleToPole = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(VActorForward.GetSafeNormal(), VToPole.GetSafeNormal())));
		FVector CrossVector = FVector::CrossProduct(VActorForward, VToPole);

		UE_LOG(LogTemp, Warning, TEXT("POLE ANGLE %f"), AngleToPole);


		if (CrossVector.Y >= 0) {
			AngleToPole = 360 - AngleToPole;
		}

		UE_LOG(LogTemp, Warning, TEXT("POLE ANGLE %f"), AngleToPole);


		// If yaw is negative then facing th bottom half of the planet
		// Character->GetSpringArm()->RelativeRotation.Yaw

		UE_LOG(LogTemp, Warning, TEXT("Cross ANGLE %s"), *CrossVector.ToCompactString());

		return AngleToPole / 360;
	}
	return 0;
}
