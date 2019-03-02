#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomActor.generated.h"

//class USkeletalMeshComponent;

UCLASS()
class PLANTTHATWHEAT_API ACustomActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACustomActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* MeshComp;
};
