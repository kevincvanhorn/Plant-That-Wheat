

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CProceduralMesh.generated.h"

class UProceduralMeshComponent;

UCLASS()
class PLANTTHATWHEAT_API ACProceduralMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACProceduralMesh();

private:
	void PostActorCreated();
	
	void PostLoad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * MeshComp;

private:
	void CreateTriangle();
	
};
	