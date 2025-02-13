#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TestGameMode.generated.h"

UCLASS()
class TEST_API ATestGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ATestGameMode();
	virtual void BeginPlay() override;
};
