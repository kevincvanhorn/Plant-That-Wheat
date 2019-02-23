

#include "CCharacter.h"
#include "CPickupWidget.h"
#include "Components/WidgetComponent.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "CGameMode.h"
#include "CPlayerState.h"
#include "Engine/Classes/GameFramework/Pawn.h"
#include "CLevelWidget_PStarting.h"
#include "CPlayerController.h"
#include "CCompassWidget.h"
#include "CToolWidget.h"

#include "CWateringTool.h"
#include "CDigTool.h"
#include "CPlanetActor.h"

#include "CLevel_PStarting.h"
#include "CLevelScriptActor.h"

ACCharacter::ACCharacter() {
	PickupWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidgetComp->SetWidgetClass(PickupWidgetClass);
	PickupWidgetComp->SetRelativeLocation(FVector(250, -90, 17));
	PickupWidgetComp->SetRelativeRotation(FRotator(180,0,0));
	PickupWidgetComp->SetRelativeScale3D(FVector(0.1,0.1,0.1));
	
	ToolWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("ToolWidget"));
	if (ToolWidgetComp && ToolWidgetClass) {
		ToolWidgetComp->SetWidgetClass(ToolWidgetClass);
	}
	ToolWidgetComp->SetupAttachment(GetCamera());
	PickupWidgetComp->SetupAttachment(GetCamera());
}

void ACCharacter::BeginPlay() {
	Super::BeginPlay();
	
	ACPlayerController* Controller = Cast<ACPlayerController>(GetController());

	if (GameMode && Controller && LevelWidgetClass && CompassWidgetClass) {

		UCCompassWidget* CompassWidget = CreateWidget<UCCompassWidget>(Controller, CompassWidgetClass);
		Level = Cast<ACLevelScriptActor>(GetWorld()->GetLevelScriptActor());
		
		if (Level) {
			if (Level->Planet) {
				Planet = Level->Planet;
				if (WateringTool) WateringTool->Init(Planet);
			}
			
			if(DigTool) DigTool->Init(Level);
		}

		if (CompassWidget) {
			CompassWidget->Init(GameMode, this);
			CompassWidget->AddToViewport();
		}

		ACLevel_PStarting* StartingLevel = Cast<ACLevel_PStarting>(Level);

		if (StartingLevel) {
			UCLevelWidget_PStarting* LevelWidget = CreateWidget<UCLevelWidget_PStarting>(Controller, LevelWidgetClass); // TODO: Make this generic for all levels
			if (LevelWidget) {
				LevelWidget->Init(GameMode, StartingLevel);
				LevelWidget->AddToViewport();
			}
		}
	}
	
	ToolWidget = Cast<UCToolWidget>(ToolWidgetComp->GetUserWidgetObject());
	if (ToolWidget) {
		ToolWidget->Init(ActiveTools.Num(), CurToolModeIndex);
	}

	//CPlayerState = Cast<ACPlayerState>(this->PlayerState);
	CPlayerState = Cast<ACPlayerState>(this->GetPlayerState());	

	UCPickupWidget* PickupWidget = Cast<UCPickupWidget>(PickupWidgetComp->GetUserWidgetObject());
	if (PickupWidget && Level && CPlayerState) {
		PickupWidget->Init(Level, CPlayerState);
	}

	// Delegate for Pickup collection: 
	/*if (GameMode) {
		// Bind to OnPlayerCollectWheat Delegate.
		GameMode->OnPlayerCollectWheat.AddUniqueDynamic(this, &ACCharacter::UpdatePickupDisplay);
	}*/
}

void ACCharacter::SwitchTool()
{
	Super::SwitchTool();
	if (ToolWidget) {
		ToolWidget->SwitchTool(CurToolModeIndex);
	}
}

void ACCharacter::OnPickupItem(ACPickupActor * Pickup)
{
	Super::OnPickupItem(Pickup);
	if (Level) { Level->OnCollectWheat.Broadcast(); }
}
