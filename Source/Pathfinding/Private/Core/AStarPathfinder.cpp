#include "Core/AStarPathfinder.h"
#include "Core/PathNode.h"
#include "Core/PathGrid.h"
#include "Containers/BinaryHeap.h"

AAStarPathfinder::AAStarPathfinder()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAStarPathfinder::SetSeekerAndTarget(AActor* seeker, AActor* target)
{
	Seeker = seeker;
	Target = target;
}

void AAStarPathfinder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FindPath(Seeker->GetActorLocation(), Target->GetActorLocation());
}

void AAStarPathfinder::SetPathGrid(APathGrid* grid)
{
	Grid = grid;
}

void AAStarPathfinder::FindPath(FVector startPos, FVector endPos)
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
			Grid->Path = RetracePath(startNode, endNode);
			UE_LOG(LogTemp, Log, TEXT("Time spend: %d"), GetUnixTime() - startTime);
			return;
		}

		for (UPathNode* neighbour : Grid->GetNeighbours(currentNode))
		{
			if(!neighbour->IsWalkable || closeSet.Contains(neighbour))
			{
				continue;
			}
			int moveCostToNeighbour = currentNode->G + GetDistance(currentNode, neighbour);
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
	return path;
}

int64 AAStarPathfinder::GetUnixTime()
{
	FDateTime timeUtc = FDateTime::UtcNow();
	return timeUtc.ToUnixTimestamp() * 1000 + timeUtc.GetMillisecond();
}
