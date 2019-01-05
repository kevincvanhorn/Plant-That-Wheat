
#pragma once

#include "CoreMinimal.h"
#include "CGroundSection.h"

enum Dimension {
	X,
	Y,
	Z
};

struct Point {
	int32 NodeIndex;
	FVector Value;
};

struct Node {
	Point* Data;
	Node* Left;
	Node* Right;
	Node* Parent;
	Dimension SortDimension; // What dimension this node was sorted on.


	float GetValue() {
		if (SortDimension == Dimension::X) {
			return Data->Value.X;
		}
		else if (SortDimension == Dimension::Y) {
			return Data->Value.Y;
		}
		return Data->Value.Z;
	}
};

/**
 * A binary tree with cells, 
 */
class PLANTTHATWHEAT_API CVectorKDTree
{
public:
	CVectorKDTree(TMap<int32, ACGroundSection::WheatInfo> PointMap);
	~CVectorKDTree();

	/** Get the index of the closest point to a Query location. */
	int32 GetNearestNeighbor(FVector Query);

protected:
	void ConstructTree(TArray<Point*> Points, int32 Low, int32 High, Dimension SortDimension);

	Node* Insert(Point* Point, Node* Cur, Node* Prev, Dimension Dim);

	void Insert(Point* Point);

	void DestroyNode(Node *Node);

	Node* Root = nullptr;

	float GetValue(Point* Point, Dimension Cur);
	
	float GetValue(Node* Node, Dimension Cur);

	float GetValue(FVector& Value, Dimension Cur);

	Dimension NextDimension(Dimension Prev);

	int32 GetMedian(TArray<Point*> Points, int32 Low, int32 High, Dimension SortDimension);
	
	Point* GetAvg(TArray<Point*> Points, int32 Low, int32 High, Dimension SortDimension);


	/* In-Place QuickSort O(nlogn): */
	void QuickSort(TArray<Point*> Points, int32 Low, int32 High, Dimension SortDimension);
	int32 Partition(TArray<Point*> Points, int32 Low, int32 High, Dimension SortDimension);
	void Swap(Point* A, Point* B);

	void PrintTree(const FString& prefix, const Node* node, bool isLeft);

	//void PruneTree(FVector Query, Node* Node);
	void Nearest(FVector Query, Node* Node);

private:
	Node* ClosestNode;
	float MinDist;
	float CurDist = 0;

	float GetDist(FVector Query, Node* Node);

	bool bWithinBoundingBox(FVector Query, Node* Node);

	int32 Count= 0;
	int32 Inserts = 0;

	void InitialLeafSearch(FVector Query);
	TSet<Node*> VisitedHash;
	void SearchNode(FVector Query, Node* Node);
};
