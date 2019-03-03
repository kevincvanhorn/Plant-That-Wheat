

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDisableCollisionActor.generated.h"

class ACPlanetActor;
class ACLevelScriptActor;
class USphereComponent;

UCLASS()
class PLANTTHATWHEAT_API ACDisableCollisionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACDisableCollisionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	ACPlanetActor* Planet;
	ACLevelScriptActor* Level;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereCollision;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
