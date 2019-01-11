#include "CHarvestTool.h"
#include "PlantThatWheat.h"
#include "CCharacterBase.h"
#include "Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Engine/StaticMesh.h"
#include "Engine/Classes/Components/BoxComponent.h"
#include "CStaticFoliageComponent.h"

#include "CPlanetActor.h"
#include "CGroundSection.h"
#include "CPickupActor.h"

ACHarvestTool::ACHarvestTool() {
	bCanSingleTrace = false;
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collider"));
	if (Collider) {
		Collider->SetupAttachment(RootComponent);
	}
}

void ACHarvestTool::BeginPlay()
{
	if (Collider) {
		Collider->OnComponentBeginOverlap.AddDynamic(this, &ACHarvestTool::OnHarvestBeginOverlap);
		//GroundCollider->OnComponentEndOverlap.AddDynamic(this, &ACShovelTool::OnEndShovelOverlap);
	}
	Super::BeginPlay();
}

void ACHarvestTool::Interact()
{
	// TODO: Check if wheat
	if (bIsActive && FoliageArray.Num() > 0) {
		UE_LOG(LogTemp, Warning, TEXT("HARVEST INTERACT"));
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (UCStaticFoliageComponent* FoliageComponent : FoliageArray)
		{
			ACPlanetActor* Planet = FoliageComponent->PlanetOwner;
			if (!Planet) { continue; } // Currently only works with wheat components b/c they are assigned a PlanetOwner.
			
			for (int32 InstIndex : FoliageComponent->GetInstancesOverlappingMesh(*Collider)) {
				//FoliageComponent->RemoveInstance(InstIndex);
				FQuat Rot;
				FTransform Loc;

				FoliageComponent->GetInstanceTransform(InstIndex, Loc, false);
				if (!Planet->HexGrid->RemoveWheatInstance(InstIndex, Rot)) { continue; }
				
				FVector Normal = Rot.GetUpVector();

				//FVector SpawnTransform = (Normal * WheatDropOffset) + Collider->GetComponentTransform().GetLocation();
				FVector SpawnTransform = (Normal * WheatDropOffset) + Loc.GetLocation();

				WheatDropItem = GetWorld()->SpawnActor<ACPickupActor>(WheatDropClass, SpawnTransform, Collider->GetComponentTransform().GetRotation().Rotator(), SpawnParams);
				
				if (WheatDropItem && Planet) {
					WheatDropItem->SetPlanet(Planet);
				}
			}
		}
	}
}

void ACHarvestTool::OnHarvestBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("HARVEST Overlap"));
	FoliageArray.Empty();
	OtherActor->GetComponents<UCStaticFoliageComponent>(FoliageArray);
}

void ACHarvestTool::Activate()
{
	Super::Activate();
	bIsActive = true;
}

void ACHarvestTool::Deactivate()
{
	bIsActive = false;
	this->SetActorHiddenInGame(true);
}
