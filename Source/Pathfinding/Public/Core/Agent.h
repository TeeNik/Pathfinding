#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Path.h"
#include "Agent.generated.h"

UCLASS()
class PATHFINDING_API AAgent : public AActor
{
	GENERATED_BODY()
	
public:	
	AAgent();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float Speed = 1.0f;
	UPROPERTY(EditAnywhere)
	float TurnSpeed = 3.0f;
	UPROPERTY(EditAnywhere)
	float TurnDistance = 20.0f;

	FPath Path;

	//TArray<FVector> Path;
	bool IsMoving;
	int PathIndex;
	FVector CurrentWaypoint;
	FVector StartPosition;
};
