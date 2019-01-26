

#include "CProjectileActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "PlantThatWheat.h"

#include "CWheatManager.h"

#include "Engine/World.h"

// Sets default values
ACProjectileActor::ACProjectileActor()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ACProjectileActor::OnHit);		// set up a notification for when this component hits something blocking
	
																					// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	// For Tracing to get Physical Component:
	//QueryParams.AddIgnoredActor(this);
	//QueryParams.bTraceComplex = true;
	//QueryParams.bReturnPhysicalMaterial = true;

	if (MeshComponent) {
		MeshComponent->SetSimulatePhysics(false);
	}
}

void ACProjectileActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	// Destroy on ground impact:
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

		if (SurfaceType == SURFACE_GROUND || SurfaceType == SURFACE_SAND) {
			UWorld* const World = GetWorld();
			if (World != NULL && WheatManager) {
				//FActorSpawnParameters ActorSpawnParams;
				//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				
				FRotator RNormal =  UKismetMathLibrary::MakeRotFromZY((Hit.Location - OtherActor->GetActorLocation()), HitComp->GetRightVector());
				FTransform SpawnTransform = FTransform(RNormal, Hit.Location, FVector::ZeroVector);
				WheatManager->TrySpawnWheat(World, SpawnTransform, SurfaceType);

				UE_LOG(LogTemp, Warning, TEXT("PROJECTILE: Try Spawn Wheat"));


				//World->SpawnActor<AStaticMeshActor>(SeedlingClass, Hit.Location, RNormal, ActorSpawnParams);
			}

			Destroy();
		}

		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		//Destroy();
	}
}