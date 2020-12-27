#include "Core/PathNode.h"

UPathNode::UPathNode()
{
}

void UPathNode::Init(bool isWalkable, FVector worldPos, int x, int y, int weight)
{
	IsWalkable = isWalkable;
	WorldPosition = worldPos;
	X = x;
	Y = y;
	Weight = weight;
}

bool operator<(const UPathNode& a, const UPathNode& b)
{
	if (a.GetF() == b.GetF())
	{
		return a.H < b.H;
	}
	return a.GetF() < b.GetF();
}