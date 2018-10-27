// UsableActor Referenced from Tom Looman: https://www.tomlooman.com/tutorial-usableactor-system-in-c/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CUsableActor.h"
#include "CGroundSection.generated.h"

class UProceduralMeshComponent;
class ACCharacterBase;

UCLASS()
class PLANTTHATWHEAT_API ACGroundSection : public ACUsableActor
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

	static ACGroundSection* CREATE(const UObject* WorldContextObject, FTransform SpawnTransform, TArray<FVector> Vertices);

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Ground Section")
	UPhysicalMaterial *SurfaceType;

	TArray<FVector> Vertices;

	/**
	* Character attempts to interact with this ground section.
	* @param Character A reference to the main Player Character base object (for getting tool type).
	*/
	virtual bool OnUsed_Implementation(ACMultiTool * Tool) override;

	virtual bool StartFocus_Implementation() override;

	virtual bool EndFocus_Implementation() override;

	void DisableOutlines();

/* Initialization Methods: */
private:
	void PostActorCreated();
	void PostLoad();

protected:
	TArray<int32> Triangles;

	void PreSpawnInitialize(TArray<FVector> Vertices);

private:
	void AddSectionTriangles();
	void CreateSectionFace();
};
