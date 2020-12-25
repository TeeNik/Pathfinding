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
	virtual void Tick(float DeltaTime) override;

private:
	int GetDistance(UPathNode* a, UPathNode* b);
	TArray<UPathNode*> RetracePath(UPathNode* startNode, UPathNode* endNode);
	TArray<FVector> NormalizePath(const TArray<UPathNode*> path);

	//UPROPERTY(EditAnywhere)
	//TSoftObjectPtr<AActor> Seeker;
	//UPROPERTY(EditAnywhere)
	//TSoftObjectPtr<AActor> Target;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<APathGrid> Grid;

	int64 GetUnixTime();
};
