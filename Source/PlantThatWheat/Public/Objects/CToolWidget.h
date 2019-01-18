

#pragma once

#include "CoreMinimal.h"
#include "UI/CTextWidget.h"
#include "CToolWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API UCToolWidget : public UCTextWidget
{
	GENERATED_BODY()
	
	// TODO: Add functionality for when more tools are added. i.e. you dont start with all of the tools, so the image
	// and local ints will need to be updated.

public:
	UFUNCTION()
		void SwitchTool(uint8 NewToolIndex);

	/** Event when tool is switched w/ 0-1 UV offset. */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Pickup")
		void OnSwitchTool(float ToolOffset);  // offset from 0-1 offset = cur/total
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	//float OffsetY;

	int32 NumTools;

	uint8 CurToolIndex;

	void Init(int32 NumTools, uint8 CurToolIndex);
};
