#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PathNode.generated.h"

UCLASS()
class PATHFINDING_API UPathNode : public UObject
{
	GENERATED_BODY()

public:

	struct FNodeSorter
	{
		bool operator()(const UPathNode& A, const UPathNode& B) const
		{
			return A < B;
		}
	};

	bool IsWalkable;
	FVector WorldPosition;

	int X;
	int Y;
	int G;
	int H;
	int Weight;

	UPROPERTY(Transient)
	UPathNode* Parent = nullptr;

	UPathNode();
	void Init(bool isWalkable, FVector worldPos, int x, int y, int weight);

	FORCEINLINE int GetF() const { return G + H; }

	friend bool operator < (const UPathNode& a, const UPathNode& b);
};
