#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AStarPathfinder.generated.h"

class APathGrid;

UCLASS()
class PATHFINDING_API UAStarPathfinder : public UObject
{
	GENERATED_BODY()

public:
	void SetPathGrid(APathGrid* grid);
	void FindPath(FVector startPos, FVector endPos);

private:
	UPROPERTY(Transient)
	APathGrid* Grid;
	
};
