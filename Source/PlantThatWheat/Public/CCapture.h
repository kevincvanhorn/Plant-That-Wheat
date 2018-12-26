

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCapture.generated.h"

UENUM(BlueprintType)
enum class EQuadrant : uint8 // xyz representation:
{
	//Top        UMETA(DisplayName = "Top"),
	//Top_B      UMETA(DisplayName = "Top_Back"),
	
	// Top Square Cupolae
	LTT	UMETA(DisplayName = "LTT"),
	LMT UMETA(DisplayName = "LMT"),
	LBT UMETA(DisplayName = "LBT"),
	MTT UMETA(DisplayName = "MTT"),
	MMT UMETA(DisplayName = "MMT"),
	MBT UMETA(DisplayName = "MBT"),
	RTT UMETA(DisplayName = "RTT"),
	RMT UMETA(DisplayName = "RMT"),
	RBT UMETA(DisplayName = "RBT"),
	
	// Middle Octagonal Prism
	LTM UMETA(DisplayName = "LTM"),
	LMM UMETA(DisplayName = "LMM"),
	LBM UMETA(DisplayName = "LBM"),
	MTM UMETA(DisplayName = "MTM"),
	MBM UMETA(DisplayName = "MBM"),
	RTM UMETA(DisplayName = "RTM"),
	RMM UMETA(DisplayName = "RMM"),
	RBM UMETA(DisplayName = "RBM"),

	// Bottom Square Cupolae
	LTB UMETA(DisplayName = "LTB"),
	LMB UMETA(DisplayName = "LMB"),
	LBB UMETA(DisplayName = "LBB"),
	MTB UMETA(DisplayName = "MTB"),
	MMB UMETA(DisplayName = "MMB"),
	MBB UMETA(DisplayName = "MBB"),
	RTB UMETA(DisplayName = "RTB"),
	RMB UMETA(DisplayName = "RMB"),
	RBB UMETA(DisplayName = "RBB")
};

// Rhombicuboctahedron: 
// +- represent the coordinate system with positive being forward and up.
// Top square cupolae (z = 1):
//    /  [+]  \		 
//	[-]  [T]  [+]		
//    \  [-]  /		
// Middle Octagonal Prism (z = 0)
//    /  [+]  \		 
//	 [-]	 [+]		
//    \  [-]  /		 
// Bot square cupolae (z = -1)
//    /  [+]  \		 
//	[-]  [B]  [+]		
//    \  [-]  /		

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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
		TArray<UTextureRenderTarget2D*> RenderTargets;

	UFUNCTION(BlueprintCallable)
		UTextureRenderTarget2D* GetRenderTargetByIndex(int32 RT_Index);

	void CreateRenderTargetArray();

	FLinearColor GetOrthonormalBaseX(int32 index);
	FLinearColor GetOrthonormalBaseY(int32 index);
	FLinearColor GetOrthonormalBaseZ(int32 index);

	TArray<FString> QuadrantNames = {
		"LTT", "LMT", "LBT", "MTT", "MMT", "MBT", "RTT", "RMT", "RBT",
		"LTM", "LMM", "LBM", "MTM", "MBM", "RTM", "RMM", "RBM",
		"LTB", "LMB", "LBB", "MTB", "MMB", "MBB", "RTB", "RMB", "RBB"
	};

	const uint8 NUM_RT = 26; //26

private:
	const int32 RT_WIDTH = 1024; //26
	const int32 RT_HEIGHT = 1024; //26

	void PreCalcOrthoBases();

	TArray<FLinearColor> OrthoBases;

	bool bIsPlayerBelow(float playerYaw); // Is the player below the middle section of the sphere?
	bool bIsWithinQuadrant(const EQuadrant &quad, const FRotator &PlayerRot);

	void SetQuadrant(const FRotator &PlayerRot);

	FRotator direction;

	/**
	* @param bSetQuadrant sets the Quadrant to "quad" when true.
	*/
	void SetOrientation(EQuadrant quad, bool bSetQuadrant);
};
