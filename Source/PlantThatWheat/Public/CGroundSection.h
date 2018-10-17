// UsableActor Referenced from Tom Looman: https://www.tomlooman.com/tutorial-usableactor-system-in-c/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGroundSection.generated.h"

class UProceduralMeshComponent;
class ACCharacterBase;

UCLASS()
class PLANTTHATWHEAT_API ACGroundSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACGroundSection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Ground Section")
	UPhysicalMaterial *SurfaceType;

	TArray<FVector> Vertices;

	/**
	* Character attempts to interact with this ground section.
	* @param Character A reference to the main Player Character base object (for getting tool type).
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ground Section")
		bool OnInteract(ACCharacterBase* Character);

	/** Character is looking at this ground section (called only with certain tool-types). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ground Section")
		bool BeginFocus();

	/** Character stops looking at this ground section. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ground Section")
		bool EndFocus();

/* Initialization Methods: */
private:
	void PostActorCreated();
	void PostLoad();

protected:
	TArray<int32> Triangles;

private:
	void AddSectionTriangles();
	void CreateSectionFace();
};
