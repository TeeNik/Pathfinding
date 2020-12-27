#include "Core/PathGrid.h"
#include "DrawDebugHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

APathGrid::APathGrid()
{
	//PrimaryActorTick.bCanEverTick = true;
}

void APathGrid::BeginPlay()
{
	Super::BeginPlay();

	NodeDiameter = NodeRadius * 2;
	GridSizeX = FMath::RoundToInt(GridWorldSize.X / NodeDiameter);
	GridSizeY = FMath::RoundToInt(GridWorldSize.Y / NodeDiameter);

	for(const FSurfaceInfo& info : SurfaceInfo)
	{
		SurfaceWeights.Emplace(info.Type, info.Weight);
	}

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
	return Grid.Get(y * GridSizeY + x);// [y * GridSizeY + x] ;
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
				neighbours.Emplace(Grid.Get(x, y));
			}
		}
	}
	return neighbours;
}

void APathGrid::CreateGrid()
{
	Grid.Init(nullptr, GridSizeX, GridSizeY);

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
			int weight = 0;

			if(isWalkable)
			{
				FHitResult surfaceHit;
				FCollisionQueryParams params;
				params.bReturnPhysicalMaterial = true;
				GetWorld()->LineTraceSingleByChannel(surfaceHit, worldPoint, worldPoint - FVector::UpVector * 100, ECC_WorldStatic, params);
				//DrawDebugLine(GetWorld(), worldPoint, worldPoint - FVector::UpVector * 100, FColor::White, false, 100);
				if (surfaceHit.IsValidBlockingHit())
				{
					UPhysicalMaterial* physMaterial = surfaceHit.PhysMaterial.Get();
					EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(physMaterial);
					weight = SurfaceWeights.Contains(surfaceType) ? SurfaceWeights[surfaceType] : 0;
				}
			}

			//int index = i * GridSizeX + j;
			UPathNode* node = NewObject<UPathNode>();
			node->Init(isWalkable, worldPoint, i, j, weight);
			Grid.Set(node, i, j);
			
			//Grid[index] = NewObject<UPathNode>();
			//Grid[index]->Init(isWalkable, worldPoint, i, j, weight);
		}
	}
}

void APathGrid::BlurWeights(int blurSize)
{
	int kernelSize = blurSize * 2 + 1;
	int kernelExtends = kernelSize / 2 - 1;

	TArray2D<int> weightsHorizontal;
	weightsHorizontal.Init(0, GridSizeX, GridSizeY);
	TArray2D<int> weightsVertical;
	weightsVertical.Init(0, GridSizeX, GridSizeY);

}

void APathGrid::DrawGrid()
{

	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(GridWorldSize/2, 10), FColor::Green, false, 100);
	FVector bounds = FVector(NodeRadius, NodeRadius, 0);
	
	for(int i = 0; i < Grid.Num(); ++i)
	{
		UPathNode* node = Grid.Get(i);
		FColor color;
		if (node->IsWalkable)
		{
			color = node->Weight > 0 ? FColor::White : FColor::Yellow;
		}
		else
		{
			color = FColor::Red;
		}

		int priority = node->IsWalkable ? 0 : -1;
		DrawDebugBox(GetWorld(), node->WorldPosition, bounds, color, false, 100, priority);
	}
}
