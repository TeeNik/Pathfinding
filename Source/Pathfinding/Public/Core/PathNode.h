#pragma once
#include "PathNode.generated.h"

USTRUCT()
struct PATHFINDING_API FPathNode
{
	GENERATED_BODY()

public:
	bool IsWalkable;
	FVector WorldPosition;

	FPathNode();
	FPathNode(bool isWalkable, FVector worldPos);
};
