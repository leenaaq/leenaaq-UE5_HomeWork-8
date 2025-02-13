#include "ObjectPoolManager.h"
#include "Engine/World.h"
#include "Spike.h"

AObjectPoolManager::AObjectPoolManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AObjectPoolManager::BeginPlay()
{
    Super::BeginPlay();
}

AActor* AObjectPoolManager::GetPooledObject(TSubclassOf<AActor> ObjectClass)
{
    if (!ObjectClass) return nullptr;

    if (ObjectPool.Contains(ObjectClass))
    {
        for (AActor* Object : ObjectPool[ObjectClass])
        {
            if (Object && Object->IsHidden())
            {
                Object->SetActorHiddenInGame(false);
                Object->SetActorEnableCollision(true);
                return Object;
            }
        }
    }

    AActor* NewObject = GetWorld()->SpawnActor<AActor>(ObjectClass);
    if (NewObject)
    {
        ObjectPool.FindOrAdd(ObjectClass).Add(NewObject);
        return NewObject;
    }

    return nullptr;
}


void AObjectPoolManager::ReturnPooledObject(AActor* Object)
{
    if (Object)
    {
        Object->SetActorHiddenInGame(true);
        Object->SetActorEnableCollision(false);
        Object->SetActorTickEnabled(false);
        Object->SetActorLocation(FVector::ZeroVector);
        Object->SetActorRotation(FRotator::ZeroRotator);
    }
}
