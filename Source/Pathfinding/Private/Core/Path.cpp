#include "Core/Path.h"

FPath::FPath()
{
}

FPath::FPath(const TArray<FVector>& waypoints, const FVector& startPos, float turnDist)
{
	LookPoints = waypoints;
	TurnBoundaries.Init(FPathLine(), LookPoints.Num());
	FinishLineIndex = TurnBoundaries.Num() - 1;
	FVector previousPoint = startPos;

	for(int i = 0; i < LookPoints.Num(); ++i)
	{
		FVector currentPoint = LookPoints[i];
		FVector dirToCurrentPoint = currentPoint - previousPoint;
		dirToCurrentPoint.Normalize();
		UE_LOG(LogTemp, Log, TEXT("%s"), *dirToCurrentPoint.ToString());
		UE_LOG(LogTemp, Log, TEXT("currentPoint %s"), *currentPoint.ToString());
		FVector turnBoundaryPoint = i == FinishLineIndex ? currentPoint : currentPoint - dirToCurrentPoint * turnDist;
		TurnBoundaries[i] = FPathLine(FVector2D(turnBoundaryPoint), FVector2D(previousPoint - dirToCurrentPoint * turnDist));
		previousPoint = turnBoundaryPoint;
	}
}

void FPath::Draw()
{

}
