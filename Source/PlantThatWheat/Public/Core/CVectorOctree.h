// Octree reference: https://github.com/brandonpelfrey/SimpleOctree/blob/master/Octree.h

#pragma once

#include "CoreMinimal.h"

class PLANTTHATWHEAT_API CVectorOctree
{
public:
	CVectorOctree(const FVector& Origin, const FVector& HalfDimension);

	CVectorOctree(const CVectorOctree& Copy);

	~CVectorOctree();
};
