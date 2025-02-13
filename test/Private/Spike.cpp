#include "Spike.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

ASpike::ASpike()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASpike::BeginPlay()
{
    Super::BeginPlay();

    InitialLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GroundLevel + DepthBelowGround);
    TargetLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GroundLevel + MaxHeight);

    SetActorLocation(InitialLocation);
}

void ASpike::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsMovingUp)
    {
        FVector NewLocation = GetActorLocation() + FVector(0, 0, SpikeSpeed * DeltaTime);
        if (NewLocation.Z >= TargetLocation.Z)
        {
            NewLocation.Z = TargetLocation.Z;
            bIsMovingUp = false;
            GetWorld()->GetTimerManager().SetTimer(StayTimerHandle, this, &ASpike::MoveSpikeDown, StayTime, false);
        }
        SetActorLocation(NewLocation);
    }
    else if (bIsMovingDown)
    {
        FVector NewLocation = GetActorLocation() - FVector(0, 0, SpikeSpeed * DeltaTime);
        if (NewLocation.Z <= InitialLocation.Z)
        {
            NewLocation.Z = InitialLocation.Z;
            bIsMovingDown = false;
            SetActorHiddenInGame(true);
            SetActorEnableCollision(false);
        }
        SetActorLocation(NewLocation);
    }
}

void ASpike::ActivateSpike()
{
    if (!bIsMovingUp && !bIsMovingDown)
    {
        SetActorHiddenInGame(false);
        SetActorEnableCollision(true);
        SetActorLocation(InitialLocation);

        bIsMovingUp = true;
        bIsMovingDown = false;

        FSoundAttenuationSettings AttenuationSettings;
        AttenuationSettings.FalloffDistance = 0.1f;
        AttenuationSettings.AttenuationShapeExtents = FVector(0.01f, 0.0f, 0.0f);

        if (SpawnSound)
        {
            UGameplayStatics::SpawnSoundAtLocation(this, SpawnSound, GetActorLocation(),
                FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, SoundAttenuation);
        }
    }
}


void ASpike::MoveSpikeDown()
{
    bIsMovingUp = false;
    bIsMovingDown = true;
}