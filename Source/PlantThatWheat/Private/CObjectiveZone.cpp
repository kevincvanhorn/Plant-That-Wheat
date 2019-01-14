

#include "CObjectiveZone.h"
#include "Components/BoxComponent.h"
#include "CCharacterBase.h"
#include "CDefaultTool.h"
#include "CMoveableActor.h"
#include "CGameMode.h"
#include "CCharacterBase.h"
#include "PlantThatWheat.h"

// Sets default values
ACObjectiveZone::ACObjectiveZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.0f));
	RootComponent = OverlapComp;

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ACObjectiveZone::OnOverlap);
}

void ACObjectiveZone::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Objective Overlap"));

	ACMoveableActor* MyActor = Cast<ACMoveableActor>(OtherActor);
	if (MyActor){
		OnOverlapMoveable(MyActor);
	}
	else {
		ACCharacterBase* MyCharacter = Cast<ACCharacterBase>(OtherActor);
		if (MyCharacter) {
			OnOverlapCharacter(MyCharacter);
		}

	}
}

void ACObjectiveZone::OnOverlapMoveable(ACMoveableActor* Moveable)
{	
}

void ACObjectiveZone::OnOverlapCharacter(ACCharacterBase * Character)
{
}

void ACObjectiveZone::OnObjectiveComplete(EPlanet CurPlanet, uint8 Objective)
{
	ACGameMode* GM = Cast<ACGameMode>(GetWorld()->GetAuthGameMode());

	if (GM) {
		GM->CompleteObjective(CurPlanet, Objective);
	}
}


