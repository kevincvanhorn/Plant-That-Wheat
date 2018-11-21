

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCapture.generated.h"

UENUM(BlueprintType)
enum class EQuadrant : uint8
{
	Top     UMETA(DisplayName = "Top"),
	Top_B      UMETA(DisplayName = "Top_B"),
	Top_F   UMETA(DisplayName = "Top_F")
};

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

	/** Creates an Orthonormal basis from the actor transform. Updates OrthonormalX,Y,Z 
		with the appropriate basis vector components. */
	UFUNCTION(BlueprintCallable)
	void CreateOrthonormalBasis();

	/** The first vector component of the orthonormal basis.*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
	FVector OrthonormalX;

	/** The second vector component of the orthonormal basis.*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
	FVector OrthonormalY;

	/** The third vector component of the orthonormal basis.*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
	FVector OrthonormalZ;

public:
	UFUNCTION(BlueprintCallable)
	bool SetCaptureOrientation(FRotator PlayerRot);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
	FRotator Orientation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	EQuadrant Quadrant;
};
