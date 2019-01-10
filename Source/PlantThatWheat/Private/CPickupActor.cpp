

#include "CPickupActor.h"
#include "CustomPhysicsActor.h"
#include "Components/SphereComponent.h"
#include "CCharacterBase.h"
#include "CMultiTool.h"

#include "CustomGravityComponent.h"
#include "PlanetActor.h"

ACPickupActor::ACPickupActor() {
	bIsActive = true;

	bUseOnOverlap = false;
	// Outline can be enabled/disable if pickup, but cannot be enabled if overlap.
	bOutlineEnabled = true; 

	OverlapComp = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapComp"));
	OverlapComp->SetSphereRadius(200.0f);
	OverlapComp->SetupAttachment(RootComponent);
}

// Pickup item via Interact:
bool ACPickupActor::OnUsed_Implementation(ACMultiTool * Tool)
{
	if (Tool && !bUseOnOverlap) {
		if (Tool->MyOwner) {
			Owner = Tool->MyOwner;
			Owner->OnPickupItem(this);
		}
	}
	else { return false; }

	return true;
}

bool ACPickupActor::StartFocus_Implementation()
{
	if(bOutlineEnabled && !bUseOnOverlap)
		MeshComponent->SetRenderCustomDepth(true);

	return true;
}

bool ACPickupActor::EndFocus_Implementation()
{
	if(bOutlineEnabled && !bUseOnOverlap)
		MeshComponent->SetRenderCustomDepth(false);

	return true;
}

void ACPickupActor::SetPlanet(APlanetActor * CurPlanet)
{
	GravityComponent->SetCurrentPlanet(CurPlanet);
}

// Pickup item via Overlap:
void ACPickupActor::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (bUseOnOverlap) {
		Super::NotifyActorBeginOverlap(OtherActor);

		UE_LOG(LogTemp, Warning, TEXT("OVERLAP"));

		Owner = Cast<ACCharacterBase>(OtherActor);
		if (Owner) {
			UE_LOG(LogTemp, Warning, TEXT("OVERLAP - VALID OWNER"));
			Owner->OnPickupItem(this);
		}
	}
}
