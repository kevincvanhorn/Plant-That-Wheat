// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define SURFACE_FLESHDEFAULT	 SurfaceType1
#define SURFACE_FLESHCRITICAL	 SurfaceType2

// Collision Channels - also need to add in Project Settings > Collision.
#define COLLISION_DEFAULTTOOL		 ECC_GameTraceChannel1
#define COLLISION_SHOVELTOOL		 ECC_GameTraceChannel2
#define COLLISION_PLANTINGTOOL		 ECC_GameTraceChannel3
#define COLLISION_HARVESTTOOL		 ECC_GameTraceChannel4