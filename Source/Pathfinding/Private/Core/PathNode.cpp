#include "Core/PathNode.h"

UPathNode::UPathNode()
{
}

void UPathNode::Init(bool isWalkable, FVector worldPos, int x, int y)
{
	IsWalkable = isWalkable;
	WorldPosition = worldPos;
	X = x;
	Y = y;
}