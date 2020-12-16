#include "Core/AStarPathfinder.h"
#include "Core/PathNode.h"
#include "Core/PathGrid.h"

void UAStarPathfinder::SetPathGrid(APathGrid* grid)
{
	Grid = grid;
}

void UAStarPathfinder::FindPath(FVector startPos, FVector endPos)
{
	FPathNode startNode = Grid->NodeFromWorldPoint(startPos);
	FPathNode endNode = Grid->NodeFromWorldPoint(endPos);

	//TList<FPathNode> openList;
	//TSet<FPathNode>
}
