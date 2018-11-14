

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCapture.generated.h"

UCLASS()
class PLANTTHATWHEAT_API ACCapture : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACCapture();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Creates an Orthonormal basis from the actor transform. */
	UFUNCTION(BlueprintCallable)
	void CreateOrthonormalBasis();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
	FVector OrthonormalX;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
	FVector OrthonormalY;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
	FVector OrthonormalZ;
};
