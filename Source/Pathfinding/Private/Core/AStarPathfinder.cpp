#include "Core/AStarPathfinder.h"
#include "Core/PathNode.h"
#include "Core/PathGrid.h"
#include "Containers/BinaryHeap.h"
#include "Algo/Reverse.h"

AAStarPathfinder::AAStarPathfinder()
{
	PrimaryActorTick.bCanEverTick = true;
}

TArray<FVector> AAStarPathfinder::FindPath(FVector startPos, FVector endPos)
{
	int64 startTime = GetUnixTime();

	UPathNode* startNode = Grid->NodeFromWorldPoint(startPos);
	UPathNode* endNode = Grid->NodeFromWorldPoint(endPos);

	UPathNode::FNodeSorter sorter;

	TSet<UPathNode*> openSet;
	TArray<UPathNode*> openHeap;
	openSet.Add(startNode);
	openHeap.HeapPush(startNode, sorter);

	TSet<UPathNode*> closeSet;

	while (openHeap.Num() > 0)
	{
		UPathNode* currentNode;
		openHeap.HeapPop(currentNode, sorter);
		openSet.Remove(currentNode);

		closeSet.Add(currentNode);

		if(currentNode == endNode)
		{
			TArray<UPathNode*> path = RetracePath(startNode, endNode);
			TArray<FVector> normalizePath = NormalizePath(path);
			UE_LOG(LogTemp, Log, TEXT("Time spend: %d"), GetUnixTime() - startTime);
			return normalizePath;
		}

		for (UPathNode* neighbour : Grid->GetNeighbours(currentNode))
		{
			if(!neighbour->IsWalkable || closeSet.Contains(neighbour))
			{
				continue;
			}
			int moveCostToNeighbour = currentNode->G + GetDistance(currentNode, neighbour) + neighbour->Weight;
			if(moveCostToNeighbour < neighbour->G || !openSet.Contains(neighbour))
			{
				neighbour->G = moveCostToNeighbour;
				neighbour->H = GetDistance(neighbour, endNode);
				neighbour->Parent = currentNode;

				if(!openSet.Contains(neighbour))
				{
					openHeap.HeapPush(neighbour, sorter);
					openSet.Add(neighbour);
				}
			}
		}
	}
	return TArray<FVector>();
}

int AAStarPathfinder::GetDistance(UPathNode* a, UPathNode* b)
{
	int distX = FMath::Abs(a->X - b->X);
	int distY = FMath::Abs(a->Y - b->Y);

	if(distX > distY)
	{
		return 14 * distY + 10 * (distX - distY);
	}
	else
	{
		return 14 * distX + 10 * (distY - distX);
	}
}

TArray<UPathNode*> AAStarPathfinder::RetracePath(UPathNode* startNode, UPathNode* endNode)
{
	TArray<UPathNode*> path;
	UPathNode* currentNode = endNode;
	while (currentNode != startNode)
	{
		path.Add(currentNode);
		currentNode = currentNode->Parent;
	}
	Algo::Reverse(path);
	return path;
}

TArray<FVector> AAStarPathfinder::NormalizePath(const TArray<UPathNode*> path)
{
	TArray<FVector> normalizedPath;
	FVector2D direction = FVector2D::ZeroVector;

	for(int i = 1; i < path.Num(); ++i)
	{
		FVector2D newDirection = FVector2D(path[i - 1]->X - path[i]->X, path[i - 1]->Y - path[i]->Y);
		if(direction != newDirection)
		{
			normalizedPath.Add(path[i-1]->WorldPosition);
		}
		direction = newDirection;
	}
	normalizedPath.Add(path[path.Num() - 1]->WorldPosition);
	return normalizedPath;
}

int64 AAStarPathfinder::GetUnixTime()
{
	FDateTime timeUtc = FDateTime::UtcNow();
	return timeUtc.ToUnixTimestamp() * 1000 + timeUtc.GetMillisecond();
}
