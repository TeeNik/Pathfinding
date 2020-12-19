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
	void SetPathGrid(APathGrid* grid);
	void FindPath(FVector startPos, FVector endPos);
	void SetSeekerAndTarget(AActor* seeker, AActor* target);

private:
	UPROPERTY(Transient)
	APathGrid* Grid;

	int GetDistance(UPathNode* a, UPathNode* b);
	TArray<UPathNode*> RetracePath(UPathNode* startNode, UPathNode* endNode);

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AActor> Seeker;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AActor> Target;
	
};
