#include "CWheatSpawnable.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "CWheatManager.h"

#include "CPlanetActor.h"


ACWheatSpawnable::ACWheatSpawnable() {
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));

	if (SceneComp && SkeletalMesh && StaticMesh) {
		RootComponent = SceneComp;
		StaticMesh->SetupAttachment(SceneComp);
		SkeletalMesh->SetupAttachment(SceneComp);
	}

	OnActorBeginOverlap.AddDynamic(this, &ACWheatSpawnable::OnBeginWheatOverlap);
}

/*void ACWheatSpawnable::OnBeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	if (!bIsHealthy) {
		//this->OnDestroy();
		Destroy();
	}
}*/

void ACWheatSpawnable::SpawnHealthy()
{
	if (!SkeletalMesh) return;

	SkeletalMesh->SetVisibility(false);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsHealthy = true;
	this->OnSpawnHealthy();
}

void ACWheatSpawnable::SpawnUnhealthy()
{
	if (!StaticMesh) return;

	StaticMesh->SetVisibility(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsHealthy = false;
	this->OnSpawnUnhealthy();
}


void ACWheatSpawnable::ConvertToInstance() {
	ACWheatManager* WheatManager = Cast<ACWheatManager>(GetOwner());
	if (!WheatManager) return;

	if (bIsHealthy) {
		//WheatManager->AddInstance_Healthy();
	}
	else {
		FTransform Transform = SkeletalMesh->GetRelativeTransform() * GetActorTransform();
		WheatManager->AddInstance_Dead(Transform);
	}
	//Destroy();
}

void ACWheatSpawnable::SetReadyToDestroy(bool bIsReady)
{
	bReadyToDestroy = bIsReady;
}

void ACWheatSpawnable::OnBeginWheatOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	if (bReadyToDestroy && !bIsHealthy) {
		if (!OtherActor->bHidden) { // TODO: Fix overlap for shovel/Harvest tool so doesn't always overlap.
			Destroy();
		}
	}
}
