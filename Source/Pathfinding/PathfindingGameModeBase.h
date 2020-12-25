#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PathfindingGameModeBase.generated.h"

class AAStarPathfinder;

UCLASS()
class PATHFINDING_API APathfindingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AAStarPathfinder> Pathfinder;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AActor> Target;
};
