#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/PathNode.h"
#include "Chaos/ChaosEngineInterface.h"
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

template<typename InElementType>
class TArray2D
{
public:
	void Init(InElementType value, int x, int y);
	InElementType& Get(int x, int y);
	InElementType& Get(int index);
	void Set(const InElementType& value, int x, int y);
	int Num() const { return SizeX* SizeY; }

protected:
	TArray<InElementType> Array;

	int SizeX = 0;
	int SizeY = 0;
};

template <typename InElementType>
void TArray2D<InElementType>::Init(InElementType value, int x, int y)
{
	Array.Init(value, x * y);
	SizeX = x;
	SizeY = y;
}

template <typename InElementType>
InElementType& TArray2D<InElementType>::Get(int x, int y)
{
	return Array[x * SizeX + y];
}

template <typename InElementType>
InElementType& TArray2D<InElementType>::Get(int index)
{
	return Array[index];
}

template <typename InElementType>
void TArray2D<InElementType>::Set(const InElementType& value, int x, int y)
{
	Array[x * SizeX + y] = value;
}

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

	void CreateGrid();
	void BlurWeights(int blurSize);
	void DrawGrid();
};
