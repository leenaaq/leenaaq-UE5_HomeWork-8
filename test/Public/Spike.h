#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h" // Timeline 사용을 위해 필요
#include "Spike.generated.h"

UCLASS()
class TEST_API ASpike : public AActor
{
    GENERATED_BODY()

public:
    ASpike();

    UFUNCTION()
    void ActivateSpike(); // 스파이크 활성화 함수

    UFUNCTION()
    void ResetSpike();


protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void UpdateSpikePosition(float Value);

    UFUNCTION()
    void OnTimelineFinished();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spike")
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spike")
    float MaxHeight; // 스파이크가 올라가는 최대 높이

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spike")
    float SpikeSpeed; // 스파이크 속도

    UPROPERTY()
    FVector InitialRelativeLocation; // 초기 위치 저장

    UPROPERTY()
    UTimelineComponent* SpikeTimeline; // 스파이크 애니메이션 타임라인

    UPROPERTY(EditAnywhere, Category = "Spike")
    UCurveFloat* SpikeCurve; // 스파이크의 애니메이션 곡선
};
