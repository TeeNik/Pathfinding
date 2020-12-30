#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PathLine.generated.h"

USTRUCT()
struct PATHFINDING_API FPathLine
{
	GENERATED_BODY()

public:
	FPathLine();
	FPathLine(const FVector2D& pointOnLine, const FVector2D& pointPerpendicularToLine);

	bool HasCrossedLine(FVector2D p);

	static const float VerticalLineGradient;
	float Gradient;
	float YIntercept;
	float GradientPerpendicular;
	FVector2D PointOnLine1;
	FVector2D PointOnLine2;
	bool ApproachSide;

	bool GetSide(FVector2D p);
};
