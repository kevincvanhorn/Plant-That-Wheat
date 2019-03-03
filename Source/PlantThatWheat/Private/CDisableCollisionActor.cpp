

#include "CDisableCollisionActor.h"

#include "CPlanetActor.h"
#include "CLevelScriptActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ACDisableCollisionActor::ACDisableCollisionActor()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	if (SphereCollision) {
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		RootComponent = SphereCollision;

		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACDisableCollisionActor::OnOverlap);
		SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ACDisableCollisionActor::OnEndOverlap);
	}
}

// Called when the game starts or when spawned
void ACDisableCollisionActor::BeginPlay()
{
	Super::BeginPlay();

	Level = Cast<ACLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	if (Level) {
		if (Level->Planet) {
			Planet = Level->Planet;
		}
	}
}

void ACDisableCollisionActor::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Disable Collision Overlap"));
	// Only pawns can trigger this event.

	if (Planet) {
		Planet->DisableSphereCollision();
	}
}

void ACDisableCollisionActor::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (Planet) {
		Planet->EnableSphereCollision();
	}
}
