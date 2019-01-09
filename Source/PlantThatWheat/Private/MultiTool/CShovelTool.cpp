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

	/*BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	if (BoxCollider) {
		BoxCollider->SetupAttachment(RootComponent);
	}*/
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

void ACShovelTool::RemoveFoliageOnOverlap()
{
	//for (UInstancedStaticMeshComponent* FoliageComponent : FoliageArray)
	for(UCStaticFoliageComponent* FoliageComponent : FoliageArray)
	{
		//DrawDebugBox(GetWorld(), GroundCollider->CalcBounds(GroundCollider->GetRelativeTransform()).GetBox().GetCenter(), GroundCollider->CalcBounds(GroundCollider->GetRelativeTransform()).GetBox.GetExtent(), FColor::Red, true, 10);

		//FVector Origin, Extent;
//		float Radius;
		//UKismetSystemLibrary::GetComponentBounds(GroundCollider, Origin, Extent, Radius);
		//Origin = GroundCollider->GetComponentLocation();
		//Extent = GroundCollider->GetComponentRotation().Vector() * (0.5 * GroundCollider->RelativeScale3D);
		//this->GetActorBounds(false, Origin, Extent);// 
		//FRotator Rotation = FRotator(0, BoxCollider->GetComponentRotation().Yaw, 0);
		//Extent = Rotation.RotateVector(Extent);

		//Origin = BoxCollider->GetComponentLocation();
		//Extent = BoxCollider->GetScaledBoxExtent(); // Extent * GetComponentTransform().GetScale3D()
		//FRotator Rotation = FRotator(0, 90, 0);
		//Extent = Rotation.RotateVector(Extent);
		
		//Rotation = FRotator(0, 90, 0);
		//Extent = Rotation.RotateVector(Extent);

		//Origin = BoxCollider->GetComponentLocation();
		//Extent = BoxCollider->GetScaledBoxExtent();

		//FBox Bounding = FBox::BuildAABB(Origin, Extent);

		//Bounding.TransformBy(BoxCollider->GetComponentTransform().ToMatrixWithScale());

		//FBox Bounding = GroundCollider->Bounds.GetBox();
		//FBox Bounding = FBox::BuildAABB(Origin, Extent);
		
		//UE_LOG(LogTemp, Warning, TEXT("Bounds Extent: %s"), *Bounding.GetExtent().ToCompactString());

		//DrawDebugBox(GetWorld(), Bounding.GetCenter(), Bounding.GetExtent(), FColor::Red, false, 10);

		//GroundCollider->CalcBounds(GroundCollider->GetComponentTransform()).GetBox()
		for (int InstIndex : FoliageComponent->GetInstancesOverlappingMesh(*GroundCollider)) {
			FoliageComponent->RemoveInstance(InstIndex);
		}
		// TODO: Fix bounding FBox.
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
	this->SetActorHiddenInGame(true);
	//Super::Deactivate();
}

