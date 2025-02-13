#include "Explosion.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "ObjectPoolManager.h"

AExplosion::AExplosion()
{
    ExplosionRadius = 300.0f;
    ExplosionDamage = 50.0f;
    ExplosionInterval = 3.0f;
    ExplosionDuration = 0.7f;
    PrimaryActorTick.bCanEverTick = false;
}

void AExplosion::BeginPlay()
{
    Super::BeginPlay();
}

void AExplosion::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        UE_LOG(LogTemp, Warning, TEXT("Explosion Overlap Detected!"));
        StartExplosion();
    }
}

void AExplosion::StartExplosion()
{
    if (!GetWorld()) return;

    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);

    if (ExplosionParticle)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ExplosionParticle,
            GetActorLocation(),
            GetActorRotation(),
            true
        );
    }

    GetWorld()->GetTimerManager().SetTimer(
        ExplosionDeactivateTimerHandle,
        this,
        &AExplosion::DeactivateExplosion,
        ExplosionDuration,
        false
    );
}


void AExplosion::DeactivateExplosion()
{
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);

    AObjectPoolManager* PoolManager = Cast<AObjectPoolManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AObjectPoolManager::StaticClass()));

    if (PoolManager)
    {
        PoolManager->ReturnPooledObject(this);
    }
}

void AExplosion::ActivateExplosion()
{
    StartExplosion();
}
