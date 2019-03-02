// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomActor.h"

//#include "Components/SkeletalMeshComponent.h"

// Sets default values
ACustomActor::ACustomActor()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	if (MeshComp) {
		RootComponent = MeshComp;
	}
}
