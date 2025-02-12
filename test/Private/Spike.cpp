#include "Spike.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

ASpike::ASpike()
{
    PrimaryActorTick.bCanEverTick = false;

    // StaticMesh ����
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    SetRootComponent(StaticMesh);

    // Timeline ����
    SpikeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SpikeTimeline"));

    MaxHeight = 100.0f;
    SpikeSpeed = 0.3f;
}

void ASpike::BeginPlay()
{
    Super::BeginPlay();

    if (StaticMesh)
    {
        // ���Ͱ� ������ �� �ʱ� ��ġ�� ����
        InitialRelativeLocation = StaticMesh->GetRelativeLocation();
        StaticMesh->SetRelativeLocation(FVector(0, 0, -50)); // ���� ��ġ�� ���� �Ʒ��� ����
    }

    if (!SpikeCurve) return;

    FOnTimelineFloat TimelineCallback;
    TimelineCallback.BindUFunction(this, FName("UpdateSpikePosition"));

    FOnTimelineEvent TimelineFinishedCallback;
    TimelineFinishedCallback.BindUFunction(this, FName("OnTimelineFinished"));

    SpikeTimeline->AddInterpFloat(SpikeCurve, TimelineCallback);
    SpikeTimeline->SetTimelineFinishedFunc(TimelineFinishedCallback);
    SpikeTimeline->SetPlayRate(SpikeSpeed);
}


void ASpike::ActivateSpike()
{
    SetActorLocation(GetActorLocation() + FVector(0, 0, -50));
    SpikeTimeline->PlayFromStart();
}




void ASpike::UpdateSpikePosition(float Value)
{
    if (!StaticMesh) return;

    float NewZ = (Value * MaxHeight);
    StaticMesh->SetRelativeLocation(FVector(0, 0, NewZ)); // �ٴڿ��� ���� �̵��ϵ��� ����
}


void ASpike::OnTimelineFinished()
{
    // �ִϸ��̼� ���� �� ó���� ���� �߰� ����
}

void ASpike::ResetSpike()
{
    if (StaticMesh)
    {
        InitialRelativeLocation = FVector(0, 0, -50); // �⺻�� �ʱ�ȭ
        StaticMesh->SetRelativeLocation(InitialRelativeLocation);
    }
}
