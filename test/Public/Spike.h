#pragma once

#include "CoreMinimal.h"
#include "ImpedimentBase.h"

#include "Spike.generated.h"

UCLASS()
class TEST_API ASpike : public AImpedimentBase
{
    GENERATED_BODY()

public:
    ASpike();

    UFUNCTION()
    void ActivateSpike();

    UFUNCTION()
    void MoveSpikeDown();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spike")
    float MaxHeight = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spike")
    float SpikeSpeed = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spike")
    float StayTime = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spike")
    float GroundLevel = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spike")
    float DepthBelowGround = -50.0f;

private:
    bool bIsMovingUp = false;
    bool bIsMovingDown = false;

    FVector InitialLocation;
    FVector TargetLocation;

    FTimerHandle StayTimerHandle;
};
