// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMultiTool.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;

UCLASS()
class PLANTTHATWHEAT_API ACMultiTool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACMultiTool();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	void PlayFireEffects(FVector TraceEnd);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
	TSubclassOf<UDamageType> DamageType; // Instead of instance. 

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
	UParticleSystem * MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
	UParticleSystem * DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
	UParticleSystem * FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MultiTool")
	UParticleSystem * TracerEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "MultiTool")
	TSubclassOf<UCameraShake> FireCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "MultiTool")
	float BaseDamage;

public:	
	UFUNCTION(BlueprintCallable, Category = "MultiTool")
	virtual void Fire();
	
};
