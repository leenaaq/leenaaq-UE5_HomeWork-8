#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "TestGameState.generated.h"

UCLASS()
class TEST_API ATestGameState : public AGameState
{
    GENERATED_BODY()

public:
    ATestGameState();
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* BackgroundMusic;
};
