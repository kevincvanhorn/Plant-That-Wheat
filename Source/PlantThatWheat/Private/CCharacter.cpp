

#include "CCharacter.h"
#include "CPickupWidget.h"
#include "Components/WidgetComponent.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "CGameMode.h"
#include "CPlayerState.h"
#include "Engine/Classes/GameFramework/Pawn.h"


ACCharacter::ACCharacter() {
	PickupWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidgetComp->SetWidgetClass(PickupWidgetClass);
	PickupWidgetComp->SetRelativeLocation(FVector(250, -90, 17));
	PickupWidgetComp->SetRelativeRotation(FRotator(180,0,0));
	PickupWidgetComp->SetRelativeScale3D(FVector(0.1,0.1,0.1));

	PickupWidgetComp->SetupAttachment(GetCamera());
}

void ACCharacter::BeginPlay() {
	Super::BeginPlay();

	// Delegate for Pickup collection: 
	/*if (GameMode) {
		// Bind to OnPlayerCollectWheat Delegate.
		GameMode->OnPlayerCollectWheat.AddUniqueDynamic(this, &ACCharacter::UpdatePickupDisplay);
	}*/
}

void ACCharacter::UpdatePickupDisplay()
{
	int32 WheatCount = -1; // Default do-nothing value.

	// Update Player State
	CPlayerState = Cast<ACPlayerState>(this->PlayerState);
	
	if (CPlayerState) {
		UE_LOG(LogTemp, Warning, TEXT("VALID PLAYER STATE"));

		CPlayerState->IncrementWheatCount();
		WheatCount = CPlayerState->GetWheatCount();
	}
	UCPickupWidget* PickupWidget = Cast<UCPickupWidget>(PickupWidgetComp->GetUserWidgetObject());
	if (PickupWidget) {
		PickupWidget->UpdateWheatCount(WheatCount);
	}
}

void ACCharacter::OnPickupItem(ACPickupActor * Pickup)
{
	Super::OnPickupItem(Pickup);

	UpdatePickupDisplay();
}
