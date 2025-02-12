#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h" // Timeline ����� ���� �ʿ�
#include "Spike.generated.h"

UCLASS()
class TEST_API ASpike : public AActor
{
    GENERATED_BODY()

public:
    ASpike();

    UFUNCTION()
    void ActivateSpike(); // ������ũ Ȱ��ȭ �Լ�

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
    float MaxHeight; // ������ũ�� �ö󰡴� �ִ� ����

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spike")
    float SpikeSpeed; // ������ũ �ӵ�

    UPROPERTY()
    FVector InitialRelativeLocation; // �ʱ� ��ġ ����

    UPROPERTY()
    UTimelineComponent* SpikeTimeline; // ������ũ �ִϸ��̼� Ÿ�Ӷ���

    UPROPERTY(EditAnywhere, Category = "Spike")
    UCurveFloat* SpikeCurve; // ������ũ�� �ִϸ��̼� �
};
