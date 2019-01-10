// Copyright 2015 Elhoussine Mehnik (Mhousse1247). All Rights Reserved.
//******************* http://ue4resources.com/ *********************//

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CUsableActor.h"
#include "CustomPhysicsActor.generated.h"

class UCustomGravityComponent;

UCLASS()
class  PLANTTHATWHEAT_API ACustomPhysicsActor : public ACUsableActor
{
	GENERATED_BODY()

public:
	/**
	* Default UObject constructor.
	*/
	ACustomPhysicsActor();

protected:
	/** StaticMesh Component*/
	UPROPERTY(Category = "CustomPhysicsActor", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComponent;

	/** Gravity Component */
	UPROPERTY(Category = "CustomPhysicsActor", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCustomGravityComponent* GravityComponent;

public:
	/** Returns StaticMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return MeshComponent; }

	/** Returns GravityComponent subobject **/
	FORCEINLINE class UCustomGravityComponent* GetGravityComponent() const { return GravityComponent; }
};
