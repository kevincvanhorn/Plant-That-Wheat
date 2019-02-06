

#pragma once

#include "CoreMinimal.h"
#include "MultiTool/CMultiTool.h"
#include "CWateringTool.generated.h"

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACWateringTool : public ACMultiTool
{
	GENERATED_BODY()
	
public:
	ACWateringTool();

	virtual void Activate() override;

	virtual void Deactivate() override;

	virtual void BeginPlay() override;

	virtual void Interact() override;
	
};
