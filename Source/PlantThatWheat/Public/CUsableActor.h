

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CUsableActor.generated.h"

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACUsableActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ACUsableActor();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable Actor")
	bool OnUsed(ACMultiTool* Tool);
		virtual bool OnUsed_Implementation(ACMultiTool* Tool);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable Actor")
	bool StartFocus();
		virtual bool StartFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable Actor")
	bool EndFocus();
		virtual bool EndFocus_Implementation();
	
};
