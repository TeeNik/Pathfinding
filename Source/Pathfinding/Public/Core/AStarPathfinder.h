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
	void SetPathGrid(APathGrid* grid);
	void FindPath(FVector startPos, FVector endPos);
	void SetSeekerAndTarget(AActor* seeker, AActor* target);
	virtual void Tick(float DeltaTime) override;

private:
	int GetDistance(UPathNode* a, UPathNode* b);
	TArray<UPathNode*> RetracePath(UPathNode* startNode, UPathNode* endNode);
	TArray<FVector> NormalizePath(const TArray<UPathNode*> path);

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AActor> Seeker;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AActor> Target;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<APathGrid> Grid;

	int64 GetUnixTime();
};
