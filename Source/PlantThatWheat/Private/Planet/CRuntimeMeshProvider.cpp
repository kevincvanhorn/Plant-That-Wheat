#include "CRuntimeMeshProvider.h"
#include "RuntimeMeshComponentPlugin.h"

FVector UCRuntimeMeshProvider::GetBoxRadius() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return BoxRadius;
}

void UCRuntimeMeshProvider::SetBoxRadius(const FVector& InRadius)
{
	FScopeLock Lock(&PropertySyncRoot);
	BoxRadius = InRadius;

	MarkAllLODsDirty();
}

UMaterialInterface* UCRuntimeMeshProvider::GetBoxMaterial() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return Material;
}

void UCRuntimeMeshProvider::SetBoxMaterial(UMaterialInterface* InMaterial)
{
	FScopeLock Lock(&PropertySyncRoot);
	Material = InMaterial;
	SetupMaterialSlot(0, FName("Cube Base"), Material);
}

void UCRuntimeMeshProvider::Initialize_Implementation()
{
	FRuntimeMeshLODProperties LODProperties;
	LODProperties.ScreenSize = 0.0f;

	ConfigureLODs({ LODProperties });

	SetupMaterialSlot(0, FName("Cube Base"), GetBoxMaterial());

	FRuntimeMeshSectionProperties Properties;
	Properties.bCastsShadow = true;
	Properties.bIsVisible = true;
	Properties.MaterialSlot = 0;
	Properties.UpdateFrequency = ERuntimeMeshUpdateFrequency::Infrequent;
	CreateSection(0, 0, Properties);

	MarkCollisionDirty();
}

FBoxSphereBounds UCRuntimeMeshProvider::GetBounds_Implementation()
{
	return FBoxSphereBounds(FBox(-BoxRadius, BoxRadius));
}

bool UCRuntimeMeshProvider::GetSectionMeshForLOD_Implementation(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData& MeshData)
{	// We should only ever be queried for section 0 and lod 0
	check(SectionId == 0 && LODIndex == 0);

	FVector BoxRadiusTemp = BoxRadius;

	float Radius = 100.0f;

	// Generate verts: X flipped from blender coords.
	FVector BoxVerts[8];
	// Z- :
	BoxVerts[0] = FVector(Radius, Radius, -Radius); // TL-
	BoxVerts[1] = FVector(Radius, -Radius, -Radius); // BL-
	BoxVerts[2] = FVector(-Radius, -Radius, -Radius);  // BR-
	BoxVerts[3] = FVector(-Radius, Radius, -Radius); // TR-
	// Z+ :
	BoxVerts[4] = FVector(Radius, Radius, Radius); // TL+
	BoxVerts[5] = FVector(Radius, -Radius, Radius); // BL+
	BoxVerts[6] = FVector(-Radius, -Radius, Radius);  // BR+
	BoxVerts[7] = FVector(-Radius, Radius, Radius); // TR+

	// Edges are in pairs of the same direction, matches UV direction left to right
	int32 BoxFaces[] = {
		0,1, 4,5, // UV Left; dir = y-
		1,2, 5,6, // UV Front; dir = x-
		2,3, 6,7, // UV Right; dir = y+
		3,0, 7,4, // UV Back; dir = x+
		5,6, 4,7, // UV Top; dir = x-
		0,3, 1,2  // UV Bot; dir = x- 
	};

	FVector BoxTangents[] = { 
		FVector(0.0f, -1.0f, 0.0f), FVector(0.0f, 0.0f, 1.0f), 
		FVector(-1.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 1.0f),
		FVector(0.0f, 1.0f, 0.0f), FVector(0.0f, 0.0f, 1.0f),
		FVector(1.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 1.0f),
		FVector(-1.0f, 0.0f, 0.0f), FVector(0.0f, 1.0f, 0.0f),
		FVector(-1.0f, 0.0f, 0.0f), FVector(0.0f, -1.0f, 0.0f),
	};

	float Frac13 = 1.0f / 3.0f;
	float Frac23 = 2.0f / 3.0f;

	FVector2D UVCoords[] = {
		FVector2D(0,Frac13), FVector2D(0.25f,Frac23), FVector2D(0,Frac13), FVector2D(0.25f,Frac23),
		FVector2D(0.25f,Frac13),  FVector2D(0.5f,Frac23), FVector2D(0.25f,Frac13), FVector2D(0.5f,Frac23),
		FVector2D(0.5f,Frac13),  FVector2D(0.75f,Frac23), FVector2D(0.5f,Frac13), FVector2D(0.75f,Frac23),
		FVector2D(0.75f,Frac13),  FVector2D(1.0f,Frac23), FVector2D(0.75f,Frac13), FVector2D(1.0f,Frac23),
		FVector2D(0.25f,0.0f),  FVector2D(0.5f,Frac13), FVector2D(0.25f,0.0f), FVector2D(0.5f,Frac13),
		FVector2D(0.25f,Frac23),  FVector2D(0.5f,1.0f), FVector2D(0.25f,Frac23), FVector2D(0.5f,1.0f),
	};

	auto AddVertex = [&](const FVector& InPosition, const FVector& InTangentX, const FVector& InTangentZ, const FVector2D& InTexCoord)
	{
		MeshData.Positions.Add(InPosition);
		MeshData.Tangents.Add(InTangentZ, InTangentX);
		MeshData.Colors.Add(FColor::White);
		MeshData.TexCoords.Add(InTexCoord);
	};	
	
	int32 Rings = 100;
	float Dist = (Radius * 2) / (float)(Rings-1);
	int32 t = 0;
	int32 vCount = 0;
	for (int32 i = 0; i < 6 * 4; i += 4) {
		for (int32 r = 0; r < Rings; ++r) {
			for (int32 s = 0; s < Rings; ++s) {
				FVector Cur = BoxVerts[BoxFaces[i]] + Dist * r * BoxTangents[t] + Dist * s * BoxTangents[t + 1];
				AddVertex(Cur.GetSafeNormal()*Radius,BoxTangents[t], BoxTangents[t + 1], UVCoords[i+2]);
			}
		}

		for (int32 r = vCount; r < vCount+(Rings * Rings)-Rings; r+=1){
			if ((r + 1) % Rings != 0) {
				MeshData.Triangles.AddTriangle(r + 1, r, r + Rings + 1);
				MeshData.Triangles.AddTriangle(r + Rings + 1, r, r + Rings);
			}
		}

		t += 2; vCount += Rings * Rings;
	}

	return true;
}

FRuntimeMeshCollisionSettings UCRuntimeMeshProvider::GetCollisionSettings_Implementation()
{
	FRuntimeMeshCollisionSettings Settings;
	Settings.bUseAsyncCooking = true;
	Settings.bUseComplexAsSimple = false;

	FVector BoxRadiusTemp = BoxRadius;
	Settings.Boxes.Emplace(BoxRadiusTemp.X * 2, BoxRadiusTemp.Y * 2, BoxRadiusTemp.Z * 2);

	return Settings;
}

bool UCRuntimeMeshProvider::HasCollisionMesh_Implementation()
{
	return true;
}

bool UCRuntimeMeshProvider::GetCollisionMesh_Implementation(FRuntimeMeshCollisionData& CollisionData)
{
	// Add the single collision section
	CollisionData.CollisionSources.Emplace(0, 5, this, 0, ERuntimeMeshCollisionFaceSourceType::Collision);

	FRuntimeMeshCollisionVertexStream& CollisionVertices = CollisionData.Vertices;
	FRuntimeMeshCollisionTriangleStream& CollisionTriangles = CollisionData.Triangles;

	FVector BoxRadiusTemp = BoxRadius;

	// Generate verts
	CollisionVertices.Add(FVector(-BoxRadiusTemp.X, BoxRadiusTemp.Y, BoxRadiusTemp.Z));
	CollisionVertices.Add(FVector(BoxRadiusTemp.X, BoxRadiusTemp.Y, BoxRadiusTemp.Z));
	CollisionVertices.Add(FVector(BoxRadiusTemp.X, -BoxRadiusTemp.Y, BoxRadiusTemp.Z));
	CollisionVertices.Add(FVector(-BoxRadiusTemp.X, -BoxRadiusTemp.Y, BoxRadiusTemp.Z));

	CollisionVertices.Add(FVector(-BoxRadiusTemp.X, BoxRadiusTemp.Y, -BoxRadiusTemp.Z));
	CollisionVertices.Add(FVector(BoxRadiusTemp.X, BoxRadiusTemp.Y, -BoxRadiusTemp.Z));
	CollisionVertices.Add(FVector(BoxRadiusTemp.X, -BoxRadiusTemp.Y, -BoxRadiusTemp.Z));
	CollisionVertices.Add(FVector(-BoxRadiusTemp.X, -BoxRadiusTemp.Y, -BoxRadiusTemp.Z));

	// Pos Z
	CollisionTriangles.Add(0, 1, 3);
	CollisionTriangles.Add(1, 2, 3);
	// Neg X
	CollisionTriangles.Add(4, 0, 7);
	CollisionTriangles.Add(0, 3, 7);
	// Pos Y
	CollisionTriangles.Add(5, 1, 4);
	CollisionTriangles.Add(1, 0, 4);
	// Pos X
	CollisionTriangles.Add(6, 2, 5);
	CollisionTriangles.Add(2, 1, 5);
	// Neg Y
	CollisionTriangles.Add(7, 3, 6);
	CollisionTriangles.Add(3, 2, 6);
	// Neg Z
	CollisionTriangles.Add(7, 6, 4);
	CollisionTriangles.Add(6, 5, 4);

	return true;
}

bool UCRuntimeMeshProvider::IsThreadSafe_Implementation()
{
	return true;
}