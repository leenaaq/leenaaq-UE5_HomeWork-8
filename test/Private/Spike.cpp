#include "Spike.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

ASpike::ASpike()
{
    PrimaryActorTick.bCanEverTick = false;

    // StaticMesh 생성
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    SetRootComponent(StaticMesh);

    // Timeline 생성
    SpikeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SpikeTimeline"));

    MaxHeight = 100.0f;
    SpikeSpeed = 0.3f;
}

void ASpike::BeginPlay()
{
    Super::BeginPlay();

    if (StaticMesh)
    {
        // 액터가 시작할 때 초기 위치를 설정
        InitialRelativeLocation = StaticMesh->GetRelativeLocation();
        StaticMesh->SetRelativeLocation(FVector(0, 0, -50)); // 시작 위치를 지면 아래로 설정
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
    StaticMesh->SetRelativeLocation(FVector(0, 0, NewZ)); // 바닥에서 위로 이동하도록 설정
}


void ASpike::OnTimelineFinished()
{
    // 애니메이션 종료 후 처리할 내용 추가 가능
}

void ASpike::ResetSpike()
{
    if (StaticMesh)
    {
        InitialRelativeLocation = FVector(0, 0, -50); // 기본값 초기화
        StaticMesh->SetRelativeLocation(InitialRelativeLocation);
    }
}
