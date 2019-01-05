#include "CVectorKDTree.h"
#include "CGroundSection.h"

CVectorKDTree::CVectorKDTree(TMap<int32, ACGroundSection::WheatInfo> PointMap)
{
	TArray<Point*> Points;

	/*Point* Arr[] = { 
		new Point{0,FVector(-1,1,1)}, 
		new Point{1,FVector(0,1,1)}, 
		new Point{2,FVector(1,1,1)},
		new Point{3,FVector(-1,0,1)}, 
		new Point{4,FVector(0,0,1)}, 
		new Point{5,FVector(1,0,1)}, 
		new Point{6,FVector(-1,-1,1)},
		new Point{7,FVector(0,-1,1)},
		new Point{8,FVector(1,-1,1)},
		
		new Point{9,FVector(-1,1,0)},
		new Point{10,FVector(0,1,0)},
		new Point{11,FVector(1,1,0)},
		new Point{12,FVector(-1,0,0)},
		new Point{13,FVector(0,0,0)},
		new Point{14,FVector(1,0,0)},
		new Point{15,FVector(-1,-1,0)},
		new Point{16,FVector(0,-1,0)},
		new Point{17,FVector(1,-1,0)},

		new Point{18,FVector(-1,1,-1)},
		new Point{19,FVector(0,1,-1)},
		new Point{20,FVector(1,1,-1)},
		new Point{21,FVector(-1,0,-1)},
		new Point{22,FVector(0,0,-1)},
		new Point{23,FVector(1,0,-1)},
		new Point{24,FVector(-1,-1,-1)},
		new Point{25,FVector(0,-1,-1)},
		new Point{26,FVector(1,-1,-1)},
	};*/

		
	for (auto Elem : PointMap) {
		Points.Emplace(new Point{ Elem.Key, *Elem.Value.Centroid });
	}

	//Points.Append(Arr, ARRAY_COUNT(Arr));

	/*QuickSort(Points, 0, Points.Num()-1, Dimension::Y);
	for (Point* p : Points) {
		UE_LOG(LogTemp, Warning, TEXT("%d"), p->NodeIndex);
	}*/

	// Construct Tree
	//ConstructTree(Points, 0, Points.Num()-1, Dimension::Z);

	for (Point* Point : Points) {
		Insert(Point);
	}

	PrintTree("", Root, false);

	//GetNearestNeighbor(FVector(1, 1, 1));
}

CVectorKDTree::~CVectorKDTree()
{
	DestroyNode(Root);
}


int32 CVectorKDTree::GetNearestNeighbor(FVector Query) {
	UE_LOG(LogTemp, Warning, TEXT("NEAREST: %s"), *Query.ToCompactString());

	ClosestNode = Root;
	MinDist = TNumericLimits<float>::Max();
	
	Count = 0;

	//Nearest(Query, Root);
	InitialLeafSearch(Query);

	//UE_LOG(LogTemp, Warning, TEXT("NODE: %d"), ClosestNode->Data->NodeIndex);
	//UE_LOG(LogTemp, Warning, TEXT("Inserts: %d"), Inserts);
	UE_LOG(LogTemp, Warning, TEXT("Count: - %d"), Count);
	return ClosestNode->Data->NodeIndex;
}

void CVectorKDTree::Nearest(FVector Query, Node* Node) {
	if (Node == nullptr) return;

	Count++;

	//UE_LOG(LogTemp, Warning, TEXT("NEAREST CHECK %d"), Node->Data->NodeIndex);
	//UE_LOG(LogTemp, Warning, TEXT("Data %s"), *Node->Data->Value.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Query %s"), *Query.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("DIST: %f"), CurDist);
	//float AxisDist = GetDist(Query, Node);
	//UE_LOG(LogTemp, Warning, TEXT("Axis: %f"), AxisDist);

	// TODO: Optimize this for each node not left AND right
	//if (bWithinBoundingBox(Query, Node)) {
		CurDist = FMath::Square(Query.X - Node->Data->Value.X) + FMath::Square(Query.Y - Node->Data->Value.Y) + FMath::Square(Query.Z - Node->Data->Value.Z);
		if (CurDist < MinDist) {
			MinDist = CurDist;
			ClosestNode = Node;
			//UE_LOG(LogTemp, Warning, TEXT("NEW MIN %f"), MinDist);
		}
		if (bWithinBoundingBox(Query, Node->Left))
			Nearest(Query, Node->Left);
		if (bWithinBoundingBox(Query, Node->Right))
			Nearest(Query, Node->Right);
	//}
}

void CVectorKDTree::InitialLeafSearch(FVector Query) {
	Node* Prev = nullptr;
	Node* Leaf = nullptr;
	Node* Node = Root;
	VisitedHash.Empty();

	while (Node != nullptr) {
		if (GetValue(Query, Node->SortDimension) < Node->SortDimension) {
			Prev = Node;
			Node = Node->Left;
		}
		else {
			Prev = Node;
			Node = Node->Right;
		}
	}
	Leaf = Prev;

	if (Leaf != nullptr) {
		Node = Leaf;
		// Search up from leaf. 
		while (Node != nullptr) {
			SearchNode(Query, Node);
			Node = Node->Parent;
		}
	}
}

void CVectorKDTree::SearchNode(FVector Query, Node* Node) {
	VisitedHash.Add(Node);

	Count++;
	CurDist = FMath::Square(Query.X - Node->Data->Value.X) + FMath::Square(Query.Y - Node->Data->Value.Y) + FMath::Square(Query.Z - Node->Data->Value.Z);
	if (CurDist < MinDist) {
		MinDist = CurDist;
		ClosestNode = Node;
		//UE_LOG(LogTemp, Warning, TEXT("NEW MIN %f"), MinDist);
	}
	if ((Node->Left) != nullptr && !VisitedHash.Contains(Node->Left) && bWithinBoundingBox(Query, Node->Left))
		SearchNode(Query, Node->Left);
	if ((Node->Right) != nullptr && !VisitedHash.Contains(Node->Right) && bWithinBoundingBox(Query, Node->Right))
		SearchNode(Query, Node->Right);
}

bool CVectorKDTree::bWithinBoundingBox(FVector Query, Node* Node) {
	if (Node == nullptr)
		return false;
	else if (FMath::Square(Query.Z - Node->Data->Value.Z) < MinDist)
		return true;
	else if (FMath::Square(Query.Y - Node->Data->Value.Y) < MinDist)
		return true;
	else if (FMath::Square(Query.X - Node->Data->Value.X) < MinDist)
		return true;
	return false;
}

float CVectorKDTree::GetDist(FVector Query, Node * Node)
{
	// Should calculate on the previous of the sort distance, b/c sortdistance is used to calculate the 'next' node.
if (Node->SortDimension == Dimension::X) {
	return FMath::Square(Query.Z - Node->Data->Value.Z);
}
else if (Node->SortDimension == Dimension::Y) {
	return FMath::Square(Query.X - Node->Data->Value.X);
}
else
return FMath::Square(Query.Y - Node->Data->Value.Y);
}


void CVectorKDTree::ConstructTree(TArray<Point*> Points, int32 Low, int32 High, Dimension SortDimension)
{
	if (Low < High) {
		int32 M = GetMedian(Points, Low, High, SortDimension); // Get the Median index and insert that node.
		ConstructTree(Points, Low, M - 1, NextDimension(SortDimension));
		ConstructTree(Points, M + 1, High, NextDimension(SortDimension));
	}
	else if (Low == High) {
		Insert(Points[Low]);
		Inserts++;
	}
}


Node* CVectorKDTree::Insert(Point* Point, Node* Cur, Node* Prev, Dimension Dim) {
	if (Cur == nullptr) {
		Cur = new Node{ Point, nullptr, nullptr, Prev, Dim };
		//UE_LOG(LogTemp, Warning, TEXT("INSERT %d"), Point->NodeIndex);
	}
	else if (GetValue(Point, Dim) < GetValue(Cur, Dim)) {
		Cur->Left = Insert(Point, Cur->Left, Cur, NextDimension(Dim));
	}
	else {
		Cur->Right = Insert(Point, Cur->Right, Cur, NextDimension(Dim));
	}
	return Cur;
}

void CVectorKDTree::Insert(Point* Point)
{
	if (Root == nullptr) {
		Root = Insert(Point, nullptr, nullptr, Dimension::Z);
	}
	else {
		Insert(Point, Root, nullptr, Dimension::Z);
	}
}

void CVectorKDTree::DestroyNode(Node *Node)
{
	if (Node != nullptr)
	{
		DestroyNode(Node->Left);
		DestroyNode(Node->Right);
		delete Node;
	}
}

float CVectorKDTree::GetValue(Point* Point, Dimension Cur) {
	if (Cur == Dimension::X) {
		return Point->Value.X;
	}
	else if (Cur == Dimension::Y) {
		return Point->Value.Y;
	}
	else if (Cur == Dimension::Z) {
		return Point->Value.Z;
	}
	return 0;
}

float CVectorKDTree::GetValue(Node * Node, Dimension Cur)
{
	if (Cur == Dimension::X) {
		return Node->Data->Value.X;
	}
	else if (Cur == Dimension::Y) {
		return Node->Data->Value.Y;
	}
	else if (Cur == Dimension::Z) {
		return Node->Data->Value.Z;
	}
	return 0;
}

float CVectorKDTree::GetValue(FVector& Value, Dimension Cur)
{
	if (Cur == Dimension::X) {
		return Value.X;
	}
	else if (Cur == Dimension::Y) {
		return Value.Y;
	}
	else if (Cur == Dimension::Z) {
		return Value.Z;
	}
	return 0;
}

Dimension CVectorKDTree::NextDimension(Dimension Prev)
{
	if (Prev == Dimension::X)
		return Dimension::Y;
	else if (Prev == Dimension::Y)
		return Dimension::Z;
	else
		return Dimension::X;
}

int32 CVectorKDTree::GetMedian(TArray<Point*> Points, int32 Low, int32 High, Dimension SortDimension)
{
	FString MStr = "";

	QuickSort(Points, Low, High, SortDimension);
	
	int32 MiddleIndex = FMath::FloorToInt((Low+High) * 0.5);
	Insert(Points[MiddleIndex]);

	Inserts++;

	//Insert(GetAvg(Points, Low, High, SortDimension));
	//UE_LOG(LogTemp, Warning, TEXT("INSERT: %d"), MiddleIndex);
	
	for (int i = Low; i <= High; i++) {
		MStr += Points[i]->Value.ToCompactString();
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *MStr);
	UE_LOG(LogTemp, Warning, TEXT("Median: %d"), MiddleIndex);

	return MiddleIndex;

}

Point* CVectorKDTree::GetAvg(TArray<Point*> Points, int32 Low, int32 High, Dimension SortDimension)
{
	float Avg = 0;
	for (int32 i = Low; i <= High; ++i) {
		Avg += GetValue(Points[i]->Value, SortDimension);
	}
	Avg /= (High - Low + 1);
	return new Point{ 0, FVector(Avg, Avg, Avg) };
}


// @author https://www.geeksforgeeks.org/quick-sort/
void CVectorKDTree::QuickSort(TArray<Point*> Points, int32 Low, int32 High, Dimension SortDimension)
{
	if (Low < High)
	{
		int32 M = Partition(Points, Low, High, SortDimension);

		QuickSort(Points, Low, M - 1, SortDimension);
		QuickSort(Points, M + 1, High, SortDimension);
	}
}

int32 CVectorKDTree::Partition(TArray<Point*> Points, int32 Low, int32 High, Dimension SortDimension)
{
	Point* Pivot = Points[High];    // pivot 
	int32 i = (Low - 1);  // Index of smaller element 

	for (int32 j = Low; j <= High - 1; j++)
	{
		// If current element is smaller than or equal to pivot 
		if (GetValue(Points[j], SortDimension) <= GetValue(Pivot, SortDimension))
		{
			i++;    // increment index of smaller element 
			Swap(Points[i], Points[j]);
		}
	}
	Swap(Points[i + 1], Points[High]);
	return (i + 1);
}

// A utility function to swap two elements 
void CVectorKDTree::Swap(Point* A, Point* B){
	Point T = *A;
	*A = *B;
	*B = T;
}

void CVectorKDTree::PrintTree(const FString& prefix, const Node* node, bool isLeft)
{
	if (node != nullptr)
	{
		FString str;
		str += prefix;
		str += (isLeft ? "|--" : " --");

		// print the value of the node
		//str += node->Data->Value.ToCompactString();
		str.AppendInt(node->Data->NodeIndex);
		str += "-";
		str.AppendInt((int32)node->SortDimension);
		str += node->Data->Value.ToCompactString();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *str);

		// enter the next tree level - left and right branch
		PrintTree(prefix + (isLeft ? "|  " : "   "), node->Left, true);
		PrintTree(prefix + (isLeft ? "|  " : "   "), node->Right, false);
	}
}