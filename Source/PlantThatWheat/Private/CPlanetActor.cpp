

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
			DynamicMaterial->SetTextureParameterValue(FName(TEXT("RT_MMT")), CaptureComp->GetRenderTargetByIndex((int32)EQuadrant::MMT));
			DynamicMaterial->SetTextureParameterValue(FName(TEXT("RT_MBT")), CaptureComp->GetRenderTargetByIndex((int32)EQuadrant::MBT));
			DynamicMaterial->SetTextureParameterValue(FName(TEXT("RT_MBM")), CaptureComp->GetRenderTargetByIndex((int32)EQuadrant::MBM));
			DynamicMaterial->SetTextureParameterValue(FName(TEXT("RT_MBB")), CaptureComp->GetRenderTargetByIndex((int32)EQuadrant::MBB));
			DynamicMaterial->SetTextureParameterValue(FName(TEXT("RT_MMB")), CaptureComp->GetRenderTargetByIndex((int32)EQuadrant::MMB));
			DynamicMaterial->SetTextureParameterValue(FName(TEXT("RT_MTB")), CaptureComp->GetRenderTargetByIndex((int32)EQuadrant::MTB));
			DynamicMaterial->SetTextureParameterValue(FName(TEXT("RT_MTM")), CaptureComp->GetRenderTargetByIndex((int32)EQuadrant::MTM));
			DynamicMaterial->SetTextureParameterValue(FName(TEXT("RT_MTT")), CaptureComp->GetRenderTargetByIndex((int32)EQuadrant::MTT));

			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MMT_X")), FLinearColor(1, 0, 0, 1));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MMT_Y")), FLinearColor(0, 1, 0, 1));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MMT_Z")), FLinearColor(0, 0, 1, 1));

			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MBT_X")), CaptureComp->GetOrthonormalBaseX((int32)EQuadrant::MBT));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MBT_Y")), CaptureComp->GetOrthonormalBaseY((int32)EQuadrant::MBT));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MBT_Z")), CaptureComp->GetOrthonormalBaseZ((int32)EQuadrant::MBT));
			
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MBM_X")), CaptureComp->GetOrthonormalBaseX((int32)EQuadrant::MBM));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MBM_Y")), CaptureComp->GetOrthonormalBaseY((int32)EQuadrant::MBM));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MBM_Z")), CaptureComp->GetOrthonormalBaseZ((int32)EQuadrant::MBM));
			
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MBB_X")), CaptureComp->GetOrthonormalBaseX((int32)EQuadrant::MBB));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MBB_Y")), CaptureComp->GetOrthonormalBaseY((int32)EQuadrant::MBB));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MBB_Z")), CaptureComp->GetOrthonormalBaseZ((int32)EQuadrant::MBB));
			
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MMB_X")), CaptureComp->GetOrthonormalBaseX((int32)EQuadrant::MMB));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MMB_Y")), CaptureComp->GetOrthonormalBaseY((int32)EQuadrant::MMB));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MMB_Z")), CaptureComp->GetOrthonormalBaseZ((int32)EQuadrant::MMB));
			
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MTB_X")), CaptureComp->GetOrthonormalBaseX((int32)EQuadrant::MTB));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MTB_Y")), CaptureComp->GetOrthonormalBaseY((int32)EQuadrant::MTB));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MTB_Z")), CaptureComp->GetOrthonormalBaseZ((int32)EQuadrant::MTB));
			
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MTM_X")), CaptureComp->GetOrthonormalBaseX((int32)EQuadrant::MTM));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MTM_Y")), CaptureComp->GetOrthonormalBaseY((int32)EQuadrant::MTM));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MTM_Z")), CaptureComp->GetOrthonormalBaseZ((int32)EQuadrant::MTM));
			
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MTT_X")), CaptureComp->GetOrthonormalBaseX((int32)EQuadrant::MTT));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MTT_Y")), CaptureComp->GetOrthonormalBaseY((int32)EQuadrant::MTT));
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_MTT_Z")), CaptureComp->GetOrthonormalBaseZ((int32)EQuadrant::MTT));

			//DynamicMaterial->SetTextureParameterValue(FName(TEXT("RT_4")), CaptureComp->GetRenderTargetByIndex((int32)EQuadrant::MMT));
			//DynamicMaterial->SetTextureParameterValue(FName(TEXT("RT_5")), CaptureComp->GetRenderTargetByIndex((int32)EQuadrant::MBT));
			//DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_X_4")), FLinearColor(1, 0, 0, 1));
			//DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_Y_4")), FLinearColor(0, 1, 0, 1));

			/*for (int i = 0; i < CaptureComp->RenderTargets.Num(); i++) {
				DynamicMaterial->SetTextureParameterValue(FName(TEXT("RT_"+i)), CaptureComp->GetRenderTargetByIndex(i));
				DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_X_"+i)), CaptureComp->GetOrthonormalBaseX(i));
				DynamicMaterial->SetVectorParameterValue(FName(TEXT("Ortho_Y_"+i)), CaptureComp->GetOrthonormalBaseY(i));
			}*/

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