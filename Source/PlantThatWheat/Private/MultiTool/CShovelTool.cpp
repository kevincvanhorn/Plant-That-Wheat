#include "CShovelTool.h"
#include "PlantThatWheat.h"
#include "CCharacterBase.h"
#include "Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"


ACShovelTool::ACShovelTool() {
	bCanSingleTrace = false;
	PrimaryActorTick.bCanEverTick = true;

	GroundCollider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundCollider"));
	if (GroundCollider){
		GroundCollider->SetupAttachment(RootComponent);
	}
}

void ACShovelTool::BeginPlay()
{
	if (GroundCollider) {
		GroundCollider->OnComponentBeginOverlap.AddDynamic(this, &ACShovelTool::OnBeginShovelOverlap);
		//GroundCollider->OnComponentEndOverlap.AddDynamic(this, &ACShovelTool::OnEndShovelOverlap);
	}
	Super::BeginPlay();
}

void ACShovelTool::OnBeginShovelOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("SHOVEL Overlap"));
	OtherActor->GetComponents<UInstancedStaticMeshComponent>(FoliageArray);
}

void ACShovelTool::OnEndShovelOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
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
	if (bIsActive && FoliageArray.Num() > 0) {
		RemoveFoliageOnOverlap();
	}
}

void ACShovelTool::Activate()
{
	Super::Activate();
	bIsActive = true;
}

void ACShovelTool::Deactivate()
{
	bIsActive = false;
	Super::Deactivate();
}

