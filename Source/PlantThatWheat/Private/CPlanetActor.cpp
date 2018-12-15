

#include "CPlanetActor.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Engine/StaticMesh.h"
#include "CPlanetProceduralMesh.h"
#include "CGroundSection.h"
#include "ProceduralMeshComponent.h"
#include "CCapture.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/TextureRenderTarget2D.h"

// ObjectInitializer Constructor used intstead of ACPlanetActor() for PlanetActor Constructor compatibility.
ACPlanetActor::ACPlanetActor(const FObjectInitializer& Objectinititializer) {
	StaticMeshScale = FVector(320,320,320);
	GroundSectionMaterial = CreateDefaultSubobject<UMaterial>(TEXT("GroundSectionMaterial"));
}

void ACPlanetActor::BeginPlay() {
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CaptureComp = GetWorld()->SpawnActor<ACCapture>(CaptureCompClass, FVector::ZeroVector, FRotator(90, 0, 0), SpawnParams);
	if (CaptureComp) {
		//CaptureComp->SetOwner(this);
		//CaptureComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		
		if (StoredMaterial != nullptr) {
			DynamicMaterial = UMaterialInstanceDynamic::Create(StoredMaterial, MeshComponent);

			for (int i = 0; i < CaptureComp->NUM_RT; i++) {
				if (i > CaptureComp->QuadrantNames.Num()) break;

				FString QName = CaptureComp->QuadrantNames[i];
				FString RT = "RT_" + QName;
				FString OX = "Ortho_" + QName + "_X";
				FString OY = "Ortho_" + QName + "_Y";
				FString OZ = "Ortho_" + QName + "_Z";

				DynamicMaterial->SetTextureParameterValue(FName(*RT), CaptureComp->GetRenderTargetByIndex(i));
				DynamicMaterial->SetVectorParameterValue(FName(*OX), CaptureComp->GetOrthonormalBaseX(i));
				DynamicMaterial->SetVectorParameterValue(FName(*OY), CaptureComp->GetOrthonormalBaseY(i));
				DynamicMaterial->SetVectorParameterValue(FName(*OZ), CaptureComp->GetOrthonormalBaseZ(i));
			}

			MeshComponent->SetMaterial(0, DynamicMaterial);
		}
	}

	return; // Comment to enable Hex grid.

	// Planet Mesh scale = Transform.Scale * PlanetMeshScale * PlanetMesh.ApproxSize;
	FTransform AdjustedTransform = GetTransform();// .SetScale3D() * PlanetMeshScale * StaticMeshScale;

	AdjustedTransform.SetScale3D(FVector(4030, 4030, 4030));
	
	if (GetWorld() != NULL) {
		ProcBoundingMesh = ACPlanetProceduralMesh::CREATE(GetWorld(), AdjustedTransform, false, false);
	}

	if (GetWorld() == NULL)
		return;

	int32 curVertex = 0; // Start at beginning of HexVertices.

	// Go through number of faces (5 or 6):
	for (int32 face = 0; face < ProcBoundingMesh->FaceSequence.Num(); face++) {
		TArray<FVector> HexVerts;
		// Go through each vertex of a face. 
		for (int32 i = 0; i < ProcBoundingMesh->FaceSequence[face]; i++) {
			if (curVertex < ProcBoundingMesh->HexVertices.Num()) {
				HexVerts.Emplace(ProcBoundingMesh->HexVertices[curVertex]);
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *ProcBoundingMesh->HexVertices[curVertex].ToString());
				curVertex++;
			}
		}

		// Create Section(from i to ProcBoundingMesh->FaceSequence -1);
		ACGroundSection::CREATE(GetWorld(), AdjustedTransform, HexVerts)->MeshComp->SetMaterial(0, GroundSectionMaterial);
	}
}