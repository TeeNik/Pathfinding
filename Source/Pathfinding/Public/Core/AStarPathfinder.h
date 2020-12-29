#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AStarPathfinder.generated.h"

class APathGrid;
class UPathNode;

UCLASS()
class PATHFINDING_API AAStarPathfinder : public AActor
{
	GENERATED_BODY()

public:
	AAStarPathfinder();
	TArray<FVector> FindPath(FVector startPos, FVector endPos);

private:
	int GetDistance(UPathNode* a, UPathNode* b);
	TArray<UPathNode*> RetracePath(UPathNode* startNode, UPathNode* endNode);
	TArray<FVector> NormalizePath(const TArray<UPathNode*> path);

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<APathGrid> Grid;
	UPROPERTY(EditAnywhere)
	float MinWaypointDist;

	int64 GetUnixTime();
};
