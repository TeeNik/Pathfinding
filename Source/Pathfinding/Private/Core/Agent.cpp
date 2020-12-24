#include "Core/Agent.h"
#include "Core/AStarPathfinder.h"
#include "Pathfinding/PathfindingGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"

AAgent::AAgent()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAgent::BeginPlay()
{
	Super::BeginPlay();

	APathfindingGameModeBase* GM = Cast<APathfindingGameModeBase>(GetWorld()->GetAuthGameMode());
	if(GM)
	{
		Path = GM->Pathfinder->FindPath(GetActorLocation(), GM->Target->GetActorLocation());
		if(Path.Num() > 0)
		{
			CurrentWaypoint = Path[0];
			IsMoving = true;
		}
	}
	
}

void AAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsMoving)
	{
		FVector actorLocation = GetActorLocation();
		if(FVector::DistSquared(actorLocation, CurrentWaypoint) < 10)
		{
			++PathIndex;
			if (PathIndex >= Path.Num())
			{
				IsMoving = false;
				return;
			}
			CurrentWaypoint = Path[PathIndex];
		}
		FVector position = UKismetMathLibrary::VInterpTo_Constant(actorLocation, CurrentWaypoint, DeltaTime, Speed);
		SetActorLocation(position);
	}
}