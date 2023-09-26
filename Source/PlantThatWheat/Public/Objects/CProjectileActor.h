// @ref: First Person Projectile Template: Epic Games


#pragma once

#include "CoreMinimal.h"
#include "CustomPhysicsActor.h"
#include "GameFramework/Actor.h"
#include "PhysicalMaterial.h"
#include "CProjectileActor.generated.h"

class ACWheatManager;

UCLASS()
class PLANTTHATWHEAT_API ACProjectileActor : public ACustomPhysicsActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
	
public:	
	// Sets default values for this actor's properties
	ACProjectileActor();

	ACWheatManager* WheatManager;

	/** Seedling class to spawn */
	//UPROPERTY(EditDefaultsOnly, Category = Projectile)
	//	TSubclassOf<AStaticMeshActor> SeedlingClass;

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

private:
	//FCollisionQueryParams QueryParams;
};
