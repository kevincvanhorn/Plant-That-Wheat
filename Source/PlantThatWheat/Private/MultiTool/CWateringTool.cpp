

#include "CWateringTool.h"
#include "PlantThatWheat.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

#include "CPlanetActor.h"

#include "Engine/Classes/Materials/MaterialInstanceDynamic.h"

#include "CDecalActor.h"
#include "Engine/World.h"

#include "Runtime/Engine/Public/TimerManager.h"

#include "CWheatSpawnable.h"

ACWateringTool::ACWateringTool() {

	bCanSingleTrace = false;
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collider"));
	if (Collider) {
		Collider->bMultiBodyOverlap = true;
		Collider->SetupAttachment(RootComponent);
	}

	WaterHitLoc = FVector::ZeroVector;

	NumDecals = 10;
	DecalDelay = 0.01f;
}

void ACWateringTool::Init(ACPlanetActor* Planet)
{
	this->Planet = Planet;
	this->PlanetLoc = Planet->GetActorLocation();

	//UMaterialInterface* MPlanet = Planet->MeshComponent->GetMaterial(0);
	this->MatPlanet = Planet->DynamicMaterial;

	InitDecalArray();
}

void ACWateringTool::Activate()
{
	Super::Activate();
}

void ACWateringTool::Deactivate()
{
	Super::Deactivate();
}

void ACWateringTool::BeginPlay()
{
	Super::BeginPlay();

	if (Collider) {
		Collider->OnComponentBeginOverlap.AddDynamic(this, &ACWateringTool::OnBeginWheatOverlap);
		Collider->OnComponentEndOverlap.AddDynamic(this, &ACWateringTool::OnEndWheatOverlap);
	}
}

void ACWateringTool::Interact()
{

}

// Not called when tool is deactivated:
void ACWateringTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceToSurface();

	if (MatPlanet->IsValidLowLevel()) {
		// Set Material
		MatPlanet->SetVectorParameterValue("WaterLoc", WaterHitLoc);
	}
}

void ACWateringTool::TraceToSurface()
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true; // Trace against each triangle of the mesh - ie. exact location. 
	QueryParams.bReturnPhysicalMaterial = true;

	FVector TraceStart = Collider->GetComponentLocation();
	FVector TraceDirection = (PlanetLoc - TraceStart).GetSafeNormal(); // TraceStart -> PlanetCenter 
	TraceStart += TraceDirection * -1 * 200; // Move up by normal so that the planet is always hit.
	FVector TraceEnd = TraceStart + (TraceDirection * SingleTraceDist); // Trace EndLocation

	FHitResult Hit; // Struct filled with hit data.
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Cyan, false, 2);
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, COLLISION_DIGTRACE, QueryParams)) {
		WaterHitLoc = Hit.Location;
		WaterHitRot = TraceDirection.ToOrientationRotator();
	}
}

void ACWateringTool::OnBeginWheatOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor) {
		ACWheatSpawnable* WheatInst = Cast<ACWheatSpawnable>(OtherActor);
		if (WheatInst) {
			WheatInst->OnBeginWatering();
		}
	}
}

void ACWateringTool::OnEndWheatOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor) {
		ACWheatSpawnable* WheatInst = Cast<ACWheatSpawnable>(OtherActor);
		if (WheatInst) {
			WheatInst->OnEndWatering();
		}
	}
}

void ACWateringTool::InitDecalArray()
{
	for (int32 i = 0; i < NumDecals; i++) {
		ACDecalActor* DecalActor = GetWorld()->SpawnActor<ACDecalActor>(DecalClass, FVector::ZeroVector, FRotator::ZeroRotator);
		DecalArray.Emplace(DecalActor);
	}

	GetWorld()->GetTimerManager().SetTimer(DecalTrailHandle, this, &ACWateringTool::UpdateDecalTrail, DecalDelay, true, 0);
}

void ACWateringTool::UpdateDecalTrail()
{
	ACDecalActor* DecalActor = DecalArray[CurDecalIndex];

	if (DecalActor) {
		// If the current decal actor is not in the process of playing an animation, "respawn" it beneath the player.
		if (DecalActor->UpdateTransform(WaterHitLoc, WaterHitRot)) {
			this->OnUpdateDecalTrail();

			if (++CurDecalIndex >= DecalArray.Num()) {
				CurDecalIndex = 0;
			}
		}
	}
}

void ACWateringTool::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}
