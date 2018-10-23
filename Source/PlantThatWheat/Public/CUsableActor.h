

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
	bool OnUsed(ACharacter* character);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable Actor")
	bool StartFocus();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable Actor")
	bool EndFocus();
	
};
