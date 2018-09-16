#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CShip.generated.h"

UCLASS()
class PLANTTHATWHEAT_API ACShip : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* MeshComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
