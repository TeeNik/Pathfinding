#include "Core/PathLine.h"

const float FPathLine::VerticalLineGradient = 10000.0f;

FPathLine::FPathLine()
{
}

FPathLine::FPathLine(const FVector2D& pointOnLine, const FVector2D& pointPerpendicularToLine)
{
	float dx = pointOnLine.X - pointPerpendicularToLine.X;
	float dy = pointOnLine.Y - pointPerpendicularToLine.Y;

	if(dx == 0)
	{
		GradientPerpendicular = VerticalLineGradient;
	}
	else
	{
		GradientPerpendicular = dy / dx;
	}

	if(GradientPerpendicular == 0)
	{
		Gradient = VerticalLineGradient;
	}
	else
	{
		Gradient = -1.0f / GradientPerpendicular;
	}

	YIntercept = pointOnLine.Y - Gradient * pointOnLine.X;
	PointOnLine1 = pointOnLine;
	PointOnLine2 = pointOnLine + FVector2D(1, Gradient);

	ApproachSide = GetSide(pointPerpendicularToLine);
}

bool FPathLine::GetSide(FVector2D p)
{
	return (p.X - PointOnLine1.X) * (PointOnLine2.Y - PointOnLine1.Y) > (p.Y - PointOnLine1.Y) * (PointOnLine2.X - PointOnLine1.X);
}

bool FPathLine::HasCrossedLine(FVector2D p)
{
	return GetSide(p) != ApproachSide;
}
