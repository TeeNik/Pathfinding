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
			FVector location = GetActorLocation();

			ECollisionChannel ECC = ECollisionChannel::ECC_WorldStatic;
			FCollisionShape CollisionShape;
			CollisionShape.ShapeType = ECollisionShape::Sphere;
			CollisionShape.SetSphere(NodeRadius);
			GetWorld()->SweepMultiByChannel(HitResults, location, location, FQuat::FQuat(), ECC, CollisionShape);

			Grid[i * GridSizeX + j] = FPathNode(HitResults.Num() == 0, worldPoint);
		}
	}

}

void APathGrid::DrawGrid()
{
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(GridWorldSize/2, 10), FColor::Green, false, 100);

	for (const FPathNode& node : Grid)
	{
		FColor color = node.IsWalkable ? FColor::White : FColor::Red;
		DrawDebugBox(GetWorld(), node.WorldPosition, FVector::OneVector * (NodeDiameter - 0.1f) /2, color, false, 100);
	}
}
