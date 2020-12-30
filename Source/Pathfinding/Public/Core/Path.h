#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PathLine.h"
#include "Path.generated.h"

USTRUCT()
struct PATHFINDING_API FPath
{
	GENERATED_BODY()

public:
	FPath();
	FPath(const TArray<FVector>& waypoints, const FVector& startPos, float turnDist);

	void Draw();

	TArray<FVector> LookPoints;
	TArray<FPathLine> TurnBoundaries;
	int FinishLineIndex;
};
