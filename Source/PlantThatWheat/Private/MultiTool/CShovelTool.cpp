#include "CShovelTool.h"
#include "PlantThatWheat.h"
#include "CCharacterBase.h"
#include "Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"


ACShovelTool::ACShovelTool() {
	PrimaryActorTick.bCanEverTick = true;

	GroundCollider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundCollider"));
	if (GroundCollider){
		GroundCollider->SetupAttachment(RootComponent);
	}
}

void ACShovelTool::BeginPlay()
{
	GroundCollider->OnComponentBeginOverlap.AddDynamic(this, &ACShovelTool::OnBeginOverlap);
	GroundCollider->OnComponentEndOverlap.AddDynamic(this, &ACShovelTool::OnEndOverlap);

	Super::BeginPlay();
}

void ACShovelTool::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	FoliageArray.Empty();
	OtherActor->GetComponents<UInstancedStaticMeshComponent>(FoliageArray);
	RemoveFoliageOnOverlap();
	++bOverlapMutex;
}

void ACShovelTool::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	--bOverlapMutex;
}

void ACShovelTool::RemoveFoliageOnOverlap()
{
	for (UInstancedStaticMeshComponent* FoliageComponent : FoliageArray)
	{
		for (int InstIndex : FoliageComponent->GetInstancesOverlappingBox(GroundCollider->Bounds.GetBox(), true)) {
			FoliageComponent->RemoveInstance(InstIndex);
		}
	}
}

void ACShovelTool::Tick(float DeltaSeconds)
{
	if (bOverlapMutex > 0) {
		RemoveFoliageOnOverlap();
	}
}

void ACShovelTool::Activate()
{
	Super::Activate();
}

void ACShovelTool::Deactivate()
{
	Super::Deactivate();
}

