

#pragma once

#include "CoreMinimal.h"
#include "CMultiTool.h"
#include "CPlantingTool.generated.h"

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACPlantingTool : public ACMultiTool
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

};
