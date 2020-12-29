#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/PathNode.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Utils/Array2D.h"
#include "PathGrid.generated.h"

USTRUCT(BlueprintType)
struct PATHFINDING_API FSurfaceInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EPhysicalSurface> Type;
	UPROPERTY(EditAnywhere)
	int Weight;
};

UCLASS()
class PATHFINDING_API APathGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	APathGrid();

	UPROPERTY(EditAnywhere)
	FVector2D GridWorldSize;
	UPROPERTY(EditAnywhere)
	float NodeRadius;
	UPROPERTY(EditAnywhere)
	int BlurSize;
	UPROPERTY(EditAnywhere)
	bool ShowDebugGrid;
	UPROPERTY(EditAnywhere)
	TArray<FSurfaceInfo> SurfaceInfo;

	UPathNode* NodeFromWorldPoint(const FVector& worldPosition);
	TArray<UPathNode*> GetNeighbours(UPathNode* node);

protected:
	virtual void BeginPlay() override;

private:
	TArray2D<UPathNode*> Grid;
	TMap<EPhysicalSurface, int> SurfaceWeights;

	float NodeDiameter;
	int GridSizeX;
	int GridSizeY;

	int MaxWeight = INT_MIN;
	int MinWeight = INT_MAX;

	void CreateGrid();
	void BlurWeights(int blurSize);
	void DrawGrid();
};
