#include "CShovelTool.h"
#include "PlantThatWheat.h"
#include "CCharacterBase.h"
#include "Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Engine/StaticMesh.h"
#include "Engine/Classes/Components/BoxComponent.h"

#include "CStaticFoliageComponent.h"


ACShovelTool::ACShovelTool() {
	bCanSingleTrace = false;
	PrimaryActorTick.bCanEverTick = true;

	GroundCollider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundCollider"));
	if (GroundCollider){
		GroundCollider->bMultiBodyOverlap = true;
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
	OtherActor->GetComponents<UCStaticFoliageComponent>(FoliageArray);
}

void ACShovelTool::OnEndShovelOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ACShovelTool::UpdateDesiredLocation(float DeltaTime)
{
	/*FRotator DesiredRot = GroundCollider->GetComponentRotation();
	float Yaw = DesiredRot.Yaw;

	Yaw = FMath::FInterpTo(PreviousYaw, Yaw, DeltaTime, CameraRotationLagSpeed);
	
	PreviousYaw = Yaw;

	GroundCollider->SetWorldRotation(FRotator(DesiredRot.Pitch, Yaw, DesiredRot.Roll));*/
}

void ACShovelTool::RemoveFoliageOnOverlap()
{
	for(UCStaticFoliageComponent* FoliageComponent : FoliageArray)
	{
		ACPlanetActor* Planet = FoliageComponent->PlanetOwner;
		for (int InstIndex : FoliageComponent->GetInstancesOverlappingMesh(*GroundCollider)) {
			FQuat Rot;
			if (Planet) { Planet->HexGrid->RemoveWheatInstance(InstIndex, Rot); }
			else { FoliageComponent->RemoveInstance(InstIndex); }
		}
	}
}

void ACShovelTool::Tick(float DeltaSeconds)
{
	if (bIsActive) {
		if (FoliageArray.Num() > 0) { RemoveFoliageOnOverlap(); }
		if (GroundCollider) { UpdateDesiredLocation(DeltaSeconds); }
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
	this->SetActorHiddenInGame(true);
	//Super::Deactivate();
}

