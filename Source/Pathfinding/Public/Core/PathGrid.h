#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/PathNode.h"
#include "PathGrid.generated.h"

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
	UPROPERTY()
	TArray<FPathNode> Grid;

	FPathNode NodeFromWorldPoint(const FVector& worldPosition);

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:

	float NodeDiameter;
	int GridSizeX;
	int GridSizeY;

	void CreateGrid();
	void DrawGrid();

};
