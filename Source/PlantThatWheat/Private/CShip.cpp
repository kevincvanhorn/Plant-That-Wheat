

#include "CShip.h"


// Sets default values
ACShip::ACShip()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	//MeshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ACShip::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

