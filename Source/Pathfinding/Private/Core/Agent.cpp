#include "Core/Agent.h"
#include "Core/AStarPathfinder.h"
#include "Pathfinding/PathfindingGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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
		TArray<FVector> waypoints = GM->Pathfinder->FindPath(GetActorLocation(), GM->Target->GetActorLocation());
		Path = FPath(waypoints, GetActorLocation(), TurnDistance);
		if(waypoints.Num() > 0)
		{
			PathIndex = 1;
			FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Path.LookPoints[PathIndex]);
			targetRotation.Roll = targetRotation.Pitch = 0;
			SetActorRotation(targetRotation);
			IsMoving = true;

			for (FPathLine line : Path.TurnBoundaries)
			{
				float length = 10;
				FVector lineDir(1, line.Gradient, 0);
				lineDir.Normalize();
				FVector lineCenter(line.PointOnLine1, 0);
				UKismetSystemLibrary::DrawDebugLine(GetWorld(), lineCenter - lineDir * length / 2, lineCenter + lineDir * length / 2, FColor::Green, 100);
			}
		}
	}
}

void AAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsMoving)
	{
		FVector actorLocation = GetActorLocation();
		FVector2D actorLoc2D = FVector2D(actorLocation);

		if (Path.TurnBoundaries[PathIndex].HasCrossedLine(actorLoc2D))
		{
			if(PathIndex == Path.FinishLineIndex)
			{
				IsMoving = false;
				return;
			}
			++PathIndex;
		}

		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(actorLocation, Path.LookPoints[PathIndex]);
		targetRotation.Roll = targetRotation.Pitch = 0;
		SetActorRotation(FMath::Lerp(GetActorRotation(), targetRotation, DeltaTime * TurnSpeed));

		//FVector position = UKismetMathLibrary::VInterpTo_Constant(actorLocation, CurrentWaypoint, DeltaTime, Speed);
		FVector position = actorLocation + GetActorForwardVector() * DeltaTime * Speed;
		SetActorLocation(position);
	}
}