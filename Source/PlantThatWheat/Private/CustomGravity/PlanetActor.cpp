// Copyright 2015 Elhoussine Mehnik (Mhousse1247). All Rights Reserved.
//******************* http://ue4resources.com/ *********************//
#include "PlanetActor.h"
#include "CustomGravityPluginPrivatePCH.h"

#include "Components/SphereComponent.h"
// Sets default values

#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "PlantThatWheat.h"



APlanetActor::APlanetActor(const FObjectInitializer& Objectinititializer) : Super(Objectinititializer)
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlanetRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent0"));
	RootComponent = PlanetRootComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent0"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName("BlockAll");
	MeshComponent->SetStaticMesh(PlanetMesh);


	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision0"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetVisibility(true);
	SphereCollision->SetHiddenInGame(true);
	SphereCollision->SetCollisionProfileName("BlockAll");
	SphereCollision->UnregisterComponent();


	// Initialization

	CollisionType = ECollisionType::ECol_Mesh;
	SphereCollisionRadius = 0.0f;
	PlanetMesh = nullptr;
	PlanetMeshScale = FVector(1.0f, 1.0f, 1.0f);
	ForceMode = EForceMode::EFM_Acceleration;
	GravityPower = 980.0f;
	bShouldUseStepping = true;

	bSphereCollisionIsSelected = (CollisionType == ECollisionType::ECol_Sphere);
}

void APlanetActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Initialization();
}

#if WITH_EDITOR

void APlanetActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	Initialization();
}

#endif //WITH_EDITOR


void APlanetActor::Initialization()
{
	bSphereCollisionIsSelected = (CollisionType == ECollisionType::ECol_Sphere);

	if (CollisionType == ECollisionType::ECol_Mesh)
	{
		if (SphereCollision->IsRegistered())
		{
			SphereCollision->UnregisterComponent();
		}

		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComponent->SetCollisionResponseToChannel(COLLISION_DIGTRACE, ECollisionResponse::ECR_Block); // 2.5.19
		MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	}
	else
	{
		// KEVIN 2.5.19
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		MeshComponent->SetCollisionResponseToChannel(COLLISION_DIGTRACE, ECollisionResponse::ECR_Block);
		MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic,ECollisionResponse::ECR_Block);

		if (!SphereCollision->IsRegistered())
		{
			SphereCollision->RegisterComponent();
		}

		SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SphereCollision->SetCollisionResponseToChannel(COLLISION_DIGTRACE, ECollisionResponse::ECR_Ignore);
		SphereCollision->SetSphereRadius(SphereCollisionRadius);
	}

	if (PlanetMesh != NULL)
	{
		MeshComponent->SetStaticMesh(PlanetMesh);
	}

	MeshComponent->SetRelativeScale3D(PlanetMeshScale);
}


FVector APlanetActor::GetGravityDirection(const FVector& TargetLocation) const
{
	return (GetActorLocation() - TargetLocation).GetSafeNormal();
}


void  APlanetActor::SetGravityPower(float NewGravity)
{
	GravityPower = NewGravity;
}



void APlanetActor::SetForceMode(EForceMode::Type newForceMode)
{
	ForceMode = newForceMode;
}

FGravityInfo APlanetActor::GetGravityinfo(const FVector& TargetLocation) const
{
	FGravityInfo GravInfo;
	GravInfo.bForceSubStepping = bShouldUseStepping;
	GravInfo.ForceMode = ForceMode;
	GravInfo.GravityPower = GravityPower;
	GravInfo.GravityDirection = GetGravityDirection(TargetLocation);

	return GravInfo;
}


