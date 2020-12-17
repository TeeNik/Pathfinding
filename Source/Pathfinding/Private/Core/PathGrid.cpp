#include "Core/PathGrid.h"
#include "DrawDebugHelpers.h"

APathGrid::APathGrid()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APathGrid::BeginPlay()
{
	Super::BeginPlay();

	NodeDiameter = NodeRadius * 2;
	GridSizeX = FMath::RoundToInt(GridWorldSize.X / NodeDiameter);
	GridSizeY = FMath::RoundToInt(GridWorldSize.Y / NodeDiameter);

	CreateGrid();
	DrawGrid();
}

FPathNode APathGrid::NodeFromWorldPoint(const FVector& worldPosition)
{
	float percentX = (worldPosition.X + GridWorldSize.X / 2) / GridWorldSize.X;
	float percentY = (worldPosition.Y + GridWorldSize.Y / 2) / GridWorldSize.Y;
	percentX = FMath::Clamp(percentX, 0.0f, 1.0f);
	percentY = FMath::Clamp(percentY, 0.0f, 1.0f);
	int x = FMath::RoundToInt((GridSizeX - 1) * percentX);
	int y = FMath::RoundToInt((GridSizeY - 1) * percentY);
	return Grid[x * GridSizeX + y];
}

void APathGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APathGrid::CreateGrid()
{
	Grid.Init(FPathNode(), GridSizeX * GridSizeY);

	FVector worldBottomLeft = GetActorLocation() - FVector::RightVector * GridWorldSize.X / 2 - FVector::ForwardVector * GridWorldSize.Y / 2;
	for (int i = 0; i < GridSizeX; ++i)
	{
		for (int j = 0; j < GridSizeY; ++j)
		{
			FVector worldPoint = worldBottomLeft + FVector::RightVector * (i * NodeDiameter + NodeRadius) + FVector::ForwardVector * (j * NodeDiameter + NodeRadius);

			TArray<FHitResult> HitResults;
			FVector startLocation = worldPoint;
			FVector endLocation = startLocation + FVector(0, 0, NodeRadius);

			ECollisionChannel ECC = ECollisionChannel::ECC_WorldStatic;
			FCollisionShape CollisionShape;
			CollisionShape.ShapeType = ECollisionShape::Sphere;
			CollisionShape.SetSphere(NodeRadius);
			bool isWalkable = !GetWorld()->SweepMultiByChannel(HitResults, worldPoint, worldPoint, FQuat::FQuat(), ECC, CollisionShape);
			Grid[i * GridSizeX + j] = FPathNode(isWalkable, worldPoint);
		}
	}
}

void APathGrid::DrawGrid()
{
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(GridWorldSize/2, 10), FColor::Green, false, 100);
	for (const FPathNode& node : Grid)
	{
		FColor color = node.IsWalkable ? FColor::White : FColor::Red;
		int priority = node.IsWalkable ? 0 : -1;
		FVector bounds = FVector(NodeRadius, NodeRadius, 10);
		DrawDebugBox(GetWorld(), node.WorldPosition, bounds, color, false, 100, priority);
	}
}
