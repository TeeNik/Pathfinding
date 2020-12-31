#include "Core/PathGrid.h"
#include "DrawDebugHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

APathGrid::APathGrid()
{
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
	return Grid.Get(y * GridSizeY + x);
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
				neighbours.Add(Grid.Get(x, y));
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
				if (surfaceHit.IsValidBlockingHit())
				{
					UPhysicalMaterial* physMaterial = surfaceHit.PhysMaterial.Get();
					EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(physMaterial);
					weight = SurfaceWeights.Contains(surfaceType) ? SurfaceWeights[surfaceType] : 0;
				}
			} 
			else
			{
				//refactor it later
				weight = SurfaceWeights[EPhysicalSurface::SurfaceType1];
			}

			UPathNode* node = NewObject<UPathNode>();
			node->Init(isWalkable, worldPoint, i, j, weight);
			Grid.Get(i, j) = node;
		}
	}

	BlurWeights(BlurSize);
}

void APathGrid::BlurWeights(int blurSize)
{
	int kernelSize = blurSize * 2 + 1;
	int kernelExtends = (kernelSize - 1) / 2;

	TArray2D<int> weightsHorizontal;
	weightsHorizontal.Init(0, GridSizeX, GridSizeY);
	TArray2D<int> weightsVertical;
	weightsVertical.Init(0, GridSizeX, GridSizeY);

	for(int y = 0; y < GridSizeY; ++y)
	{
		for(int x = - kernelExtends; x <= kernelExtends; ++x)
		{
			int sampleX = FMath::Clamp(x, 0, kernelExtends);
			weightsHorizontal.Get(0, y) += Grid.Get(sampleX, y)->Weight;
		}

		for(int x = 1; x < GridSizeX; ++x)
		{
			int removeIndex = FMath::Clamp(x - kernelExtends - 1, 0, GridSizeX);
			int addIndex = FMath::Clamp(x + kernelExtends, 0, GridSizeX - 1);
			weightsHorizontal.Get(x, y) = weightsHorizontal.Get(x - 1, y) - Grid.Get(removeIndex, y)->Weight + Grid.Get(addIndex, y)->Weight;
		}
	}

	for (int x = 0; x < GridSizeX; ++x)
	{
		for (int y = -kernelExtends; y <= kernelExtends; ++y)
		{
			int sampleY = FMath::Clamp(y, 0, kernelExtends);
			weightsVertical.Get(x, 0) += weightsHorizontal.Get(x, sampleY);
		}

		for (int y = 1; y < GridSizeY; ++y)
		{
			int removeIndex = FMath::Clamp(y - kernelExtends - 1, 0, GridSizeY);
			int addIndex = FMath::Clamp(y + kernelExtends, 0, GridSizeY - 1);
			weightsVertical.Get(x, y) = weightsVertical.Get(x, y-1) - weightsHorizontal.Get(x, removeIndex) + weightsHorizontal.Get(x, addIndex);

			int blurredWeight = FMath::RoundToInt((float)weightsVertical.Get(x, y) / (kernelSize * kernelSize));
			Grid.Get(x, y)->Weight = blurredWeight;

			if(MaxWeight < blurredWeight)
			{
				MaxWeight = blurredWeight;
			}
			if(MinWeight > blurredWeight)
			{
				MinWeight = blurredWeight;
			}
		}
	}

}

void APathGrid::DrawGrid()
{
	FVector bounds = FVector(NodeRadius, NodeRadius, 0);
	
	for(int i = 0; i < Grid.Num(); ++i)
	{
		UPathNode* node = Grid.Get(i);
		FColor color = FColor::Red;
		if (node->IsWalkable)
		{
			float c = 255 * FMath::Lerp(1.0f, 0.0f, UKismetMathLibrary::NormalizeToRange(node->Weight, MinWeight, MaxWeight));
			color = FColor(c,c,c);
		}
		if(ShowDebugGrid)
		{
			UKismetSystemLibrary::DrawDebugPlane(GetWorld(), FPlane(0, 0, 1, 50), node->WorldPosition, NodeRadius, color, 100);
		}
	}
}
