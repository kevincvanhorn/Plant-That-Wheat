

#include "CSeedThrower.h"
#include "CProjectileActor.h"
#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PlantThatWheat.h"
#include "CPlanetActor.h"
#include "CCharacterBase.h"

#include "CWheatManager.h"
#include "CLevelScriptActor.h"

ACSeedThrower::ACSeedThrower() {
	bCanDamage = false;
	MuzzleSocketName = "MuzzleFlashSocket";
}

void ACSeedThrower::Activate()
{
	Super::Activate();
	if (MyOwner) {
		MyOwner->BeginZoom();
	}
}

void ACSeedThrower::Deactivate()
{
	if (MyOwner) {
		MyOwner->EndZoom();
	}
	Super::Deactivate();
}

void ACSeedThrower::Interact()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			FVector SpawnLocation;
			FRotator SpawnRotation;

			if (OwnerController == NULL)
				return;

			OwnerController->GetActorEyesViewPoint(SpawnLocation, SpawnRotation);

			FVector  MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
			SpawnLocation = MuzzleLocation;

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			ACProjectileActor* Seed = World->SpawnActor<ACProjectileActor>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			Seed->WheatManager = WheatManager; // TODO move this to pre-begin-play.
		}
	}

	// try and play the sound if specified
	/*if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}*/

	// try and play a firing animation if specified
	/*if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}*/
}

void ACSeedThrower::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	ACLevelScriptActor* Level = Cast<ACLevelScriptActor>(World->GetLevelScriptActor());
	if (World != NULL && Level) {
		//WheatManager = Level->WheatManager;
		WheatManager = World->SpawnActor<ACWheatManager>();

		if (WheatManager && SeedlingClass) {
			// Wheat Manager is in the level, but the meshes are assigned from the tool blueprint.
			// This is to keep folige components connected to the level not the tool.
			WheatManager->SeedlingClass = SeedlingClass;
			//WheatManager->WheatMesh_Dead = WheatMesh_Dead;
			WheatManager->SetFoliageMesh_Dead(WheatMesh_Dead);

			UE_LOG(LogTemp, Warning, TEXT("SEED THROWER: Valid Class"));

			WheatManager->ActorsToIgnore = {this}; //MyOwner, MyOwner->HarvestTool, MyOwner->ShovelTool,
		}
	}
}
