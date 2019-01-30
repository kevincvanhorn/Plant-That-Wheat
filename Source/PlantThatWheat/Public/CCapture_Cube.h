

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCapture_Cube.generated.h"

UENUM(BlueprintType)
enum class EQuadrant_Cube : uint8
{
	TOP		UMETA(DisplayName = "Top"),
	BOT		UMETA(DisplayName = "Bot"),
	RIGHT	UMETA(DisplayName = "Right"),
	LEFT	UMETA(DisplayName = "Left"),
	FORWARD UMETA(DisplayName = "Forward"),
	BACK	UMETA(DisplayName = "Back")
};


UCLASS()
class PLANTTHATWHEAT_API ACCapture_Cube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACCapture_Cube();

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
		EQuadrant_Cube Quadrant;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
		TArray<UTextureRenderTarget2D*> RenderTargets;

	UFUNCTION(BlueprintCallable)
		UTextureRenderTarget2D* GetRenderTargetByIndex(int32 RT_Index);

	void CreateRenderTargetArray();

	FLinearColor GetOrthonormalBaseX(int32 index);
	FLinearColor GetOrthonormalBaseY(int32 index);
	FLinearColor GetOrthonormalBaseZ(int32 index);

	TArray<FString> QuadrantNames = {
		"TOP", "BOT", "RIGHT", "LEFT", "FORWARD", "BACK"
	};

	const uint8 NUM_RT = 6; //26

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Gameplay")
	int32 RT_WIDTH = 1024; //26
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Gameplay")
	int32 RT_HEIGHT = 1024; //26

	UPROPERTY(BlueprintReadOnly)
	UMaterialInstanceDynamic* DynamicMaterial;

private:

	void PreCalcOrthoBases();

	TArray<FLinearColor> OrthoBases;

	bool bIsPlayerBelow(float playerYaw); // Is the player below the middle section of the sphere?
	bool bIsWithinQuadrant(const EQuadrant_Cube &quad, const FRotator &PlayerRot);

	void SetQuadrant(const FRotator &PlayerRot);

	FRotator direction;

	/**
	* @param bSetQuadrant sets the Quadrant to "quad" when true.
	*/
	void SetOrientation(EQuadrant_Cube quad, bool bSetQuadrant);
	
};
