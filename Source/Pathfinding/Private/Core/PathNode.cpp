#include "Core/PathNode.h"

FPathNode::FPathNode()
{
}

FPathNode::FPathNode(bool isWalkable, FVector worldPos)
{
	IsWalkable = isWalkable;
	WorldPosition = worldPos;
}
