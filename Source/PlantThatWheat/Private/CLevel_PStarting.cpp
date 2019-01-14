

#include "CLevel_PStarting.h"
#include "CLevelWidget_PStarting.h"
#include "WidgetComponent.h"

ACLevel_PStarting::ACLevel_PStarting() {
	//LevelWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	
	/*if (LevelWidgetComp) {
		LevelWidgetComp->SetWidgetClass(LevelWidgetClass);
		LevelWidgetComp->SetupAttachment(RootComponent);
	}*/

	//UCLevelWidget_PStarting* LevelWidget = Cast<UCLevelWidget_PStarting>(LevelWidgetComp->GetUserWidgetObject());
}

void ACLevel_PStarting::BeginPlay()
{
	
}
