

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "CUsableActor.generated.h"

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API ACUsableActor : public AActor
{
	GENERATED_BODY()

public:
	ACUsableActor();
	
	// True if functional has been implemented by subclass.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable Actor")
	bool OnUsed(ACMultiTool* Tool);
		virtual bool OnUsed_Implementation(ACMultiTool* Tool);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable Actor")
	bool StartFocus();
		virtual bool StartFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable Actor")
	bool EndFocus();
		virtual bool EndFocus_Implementation();
	
	/** Does the Actor support rendering outlines? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Usable Actor")
		bool bOutlineEnabled;
};
