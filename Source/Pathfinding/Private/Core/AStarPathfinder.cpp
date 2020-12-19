#include "Core/AStarPathfinder.h"
#include "Core/PathNode.h"
#include "Core/PathGrid.h"

void AAStarPathfinder::SetSeekerAndTarget(AActor* seeker, AActor* target)
{
	Seeker = seeker;
	Target = target;
}

void AAStarPathfinder::SetPathGrid(APathGrid* grid)
{
	Grid = grid;
}

void AAStarPathfinder::FindPath(FVector startPos, FVector endPos)
{
	UPathNode* startNode = Grid->NodeFromWorldPoint(startPos);
	UPathNode* endNode = Grid->NodeFromWorldPoint(endPos);

	TArray<UPathNode*> openSet;
	TSet<UPathNode*> closeSet;
	openSet.Emplace(startNode);

	while (openSet.Num() > 0)
	{
		UPathNode* currentNode = openSet[0];
		for(int i = 1; i < openSet.Num(); ++i)
		{
			if(openSet[i]->GetF() < currentNode->GetF() || (openSet[i]->GetF() == currentNode->GetF() && openSet[i]->H < currentNode->H))
			{
				currentNode = openSet[i];
			}
		}

		openSet.Remove(currentNode);
		closeSet.Add(currentNode);

		if(currentNode == endNode)
		{
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

	if(distX < distY)
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
	while (currentNode == startNode)
	{
		path.Add(currentNode);
		currentNode = currentNode->Parent;
	}
	return path;
}
