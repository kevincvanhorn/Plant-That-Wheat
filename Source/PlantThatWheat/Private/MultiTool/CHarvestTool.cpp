#include "CHarvestTool.h"
#include "PlantThatWheat.h"
#include "CCharacterBase.h"
#include "Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Engine/StaticMesh.h"
#include "Engine/Classes/Components/BoxComponent.h"


ACHarvestTool::ACHarvestTool() {
	bCanSingleTrace = false;
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundCollider"));
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
		FBox Bounding = Collider->Bounds.GetBox();

		for (UInstancedStaticMeshComponent* FoliageComponent : FoliageArray)
		{
			for (int InstIndex : FoliageComponent->GetInstancesOverlappingBox(Bounding, true)) {
				FoliageComponent->RemoveInstance(InstIndex);
			}
		}
		//TODO: Fix Bounding FBox
	}
}

void ACHarvestTool::OnHarvestBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	OtherActor->GetComponents<UInstancedStaticMeshComponent>(FoliageArray);
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
