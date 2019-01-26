#include "CWheatSpawnable.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

ACWheatSpawnable::ACWheatSpawnable() {
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));


	if (SceneComp && SkeletalMesh && StaticMesh) {
		RootComponent = SceneComp;
		StaticMesh->SetupAttachment(SceneComp);
		SkeletalMesh->SetupAttachment(SceneComp);
	}
}

void ACWheatSpawnable::SpawnHealthy()
{
	SkeletalMesh->SetVisibility(false);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->OnSpawnHealthy();
}

void ACWheatSpawnable::SpawnUnhealthy()
{
	StaticMesh->SetVisibility(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->OnSpawnUnhealthy();
}

void ACWheatSpawnable::Init(bool bIsHealthy) {
	this->bIsHealthy = bIsHealthy;
}