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

UPathNode* APathGrid::NodeFromWorldPoint(const FVector& worldPosition)
{
	float percentX = (worldPosition.X + GridWorldSize.X / 2) / GridWorldSize.X;
	float percentY = (worldPosition.Y + GridWorldSize.Y / 2) / GridWorldSize.Y;
	percentX = FMath::Clamp(percentX, 0.0f, 1.0f);
	percentY = FMath::Clamp(percentY, 0.0f, 1.0f);
	int x = FMath::RoundToInt((GridSizeX - 1) * percentX);
	int y = FMath::RoundToInt((GridSizeY - 1) * percentY);
	return Grid[y * GridSizeX + x];
}

TArray<UPathNode*> APathGrid::GetNeighbours(UPathNode* node)
{
	TArray<UPathNode*> neighbours;
	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			if(i == 0 && j == 0)
				continue;

			int x = node->X + i;
			int y = node->Y + j;
			if(x >= 0 && x < GridSizeX && y >= 0 && y < GridSizeY)
			{
				neighbours.Emplace(Grid[x * GridSizeX + y]);
			}
		}
	}
	return neighbours;
}

void APathGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector bounds = FVector(NodeRadius, NodeRadius, 10);
	for (UPathNode* node : Path)
	{
		DrawDebugBox(GetWorld(), node->WorldPosition, bounds, FColor::Green, false, -1, -2);
	}

	for (int i = 0; i < NormalizedPath.Num(); ++i)
	{
		DrawDebugBox(GetWorld(), NormalizedPath[i], bounds, FColor::Blue, false, -1, -3);
	}

}

void APathGrid::CreateGrid()
{
	Grid.Init(nullptr, GridSizeX * GridSizeY);

	FVector worldBottomLeft = GetActorLocation() - FVector::RightVector * GridWorldSize.X / 2 - FVector::ForwardVector * GridWorldSize.Y / 2;
	for (int i = 0; i < GridSizeX; ++i)
	{
		for (int j = 0; j < GridSizeY; ++j)
		{
			FVector worldPoint = worldBottomLeft + FVector::RightVector * (i * NodeDiameter + NodeRadius) + FVector::ForwardVector * (j * NodeDiameter + NodeRadius);

			TArray<FHitResult> HitResults;
			FCollisionShape CollisionShape;
			CollisionShape.ShapeType = ECollisionShape::Sphere;
			CollisionShape.SetSphere(NodeRadius);
			bool isWalkable = !GetWorld()->SweepMultiByChannel(HitResults, worldPoint, worldPoint, FQuat(), ECC_WorldStatic, CollisionShape);
			int index = i * GridSizeX + j;
			Grid[index] = NewObject<UPathNode>();
			Grid[index]->Init(isWalkable, worldPoint, i, j);
		}
	}
}

void APathGrid::DrawGrid()
{

	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(GridWorldSize/2, 10), FColor::Green, false, 100);
	FVector bounds = FVector(NodeRadius, NodeRadius, 10);
	for (const UPathNode* node : Grid)
	{
		FColor color = node->IsWalkable ? FColor::White : FColor::Red;
		int priority = node->IsWalkable ? 0 : -1;
		DrawDebugBox(GetWorld(), node->WorldPosition, bounds, color, false, 100, priority);
	}
}
