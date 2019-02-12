#include "CWheatSpawnable.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "CWheatManager.h"

#include "Runtime/Engine/Public/TimerManager.h"

#include "PlantThatWheat.h"

#include "CPlanetActor.h"


ACWheatSpawnable::ACWheatSpawnable() {
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SeedlingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SeedlingMesh"));
	WheatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheatMesh"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));

	if (SceneComp && SkeletalMesh && SeedlingMesh && WheatMesh) {
		RootComponent = SceneComp;
		SeedlingMesh->SetupAttachment(SceneComp);
		SkeletalMesh->SetupAttachment(SceneComp);
		WheatMesh->SetupAttachment(SceneComp);
	}

	OnActorBeginOverlap.AddDynamic(this, &ACWheatSpawnable::OnBeginWheatOverlap);

	bIsFullyGrown = false;
	TimeToWater = 2.0;
	TimeWatered = 0;
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

	WheatMesh->SetVisibility(false);
	WheatMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bIsHealthy = true;
	this->OnSpawnHealthy();
}

void ACWheatSpawnable::SpawnUnhealthy()
{
	if (!SeedlingMesh) return;

	SeedlingMesh->SetVisibility(false);
	SeedlingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WheatMesh->SetVisibility(false);
	WheatMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

void ACWheatSpawnable::OnBeginWatering()
{
	UWorld* World = GetWorld();
	if (World) {
		if (bIsFullyGrown) {
			// Do nothing.
		}
		// Timer is paused:
		else if (World->GetTimerManager().IsTimerPaused(WateringHandle)) {
			World->GetTimerManager().UnPauseTimer(WateringHandle);
		}
		// Create new timer:
		else {
			World->GetTimerManager().SetTimer(WateringHandle, this, &ACWheatSpawnable::OnFullyWatered, TimeToWater, false);
		}
	}
}

void ACWheatSpawnable::OnEndWatering()
{
	UWorld* World = GetWorld();
	if (World) {
		if (World->GetTimerManager().IsTimerActive(WateringHandle)) {
			World->GetTimerManager().PauseTimer(WateringHandle);
			TimeWatered = World->GetTimerManager().GetTimerElapsed(WateringHandle);

			UE_LOG(LogTool, Warning, TEXT("CWheatSpawnable: @TimeWatered = %f"), TimeWatered);
		}
	}
}

void ACWheatSpawnable::OnFullyWatered()
{
	WheatMesh->SetVisibility(true);
	WheatMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //TODO: Adjust this to the correct collision channels.

	bIsFullyGrown = true;
	UE_LOG(LogTool, Warning, TEXT("CWheatSpawnable: OnFullyWatered() : Fully Watered!"));

	this->OnFullyWateredBP();
}

void ACWheatSpawnable::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UWorld* World = GetWorld();
	if (World)
		World->GetTimerManager().ClearAllTimersForObject(this);
}
