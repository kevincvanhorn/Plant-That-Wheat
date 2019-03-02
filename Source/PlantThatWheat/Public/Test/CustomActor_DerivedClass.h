// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomActor.h"
#include "CustomActor_DerivedClass.generated.h"

UCLASS()
class PLANTTHATWHEAT_API ACustomActor_DerivedClass : public ACustomActor
{
	GENERATED_BODY()

protected:
	void TestFunction();
};
