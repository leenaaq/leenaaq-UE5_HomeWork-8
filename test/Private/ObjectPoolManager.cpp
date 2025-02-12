#include "ObjectPoolManager.h"
#include "Engine/World.h"

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
                FVector NewLocation = FVector(FMath::RandRange(-500, 500), FMath::RandRange(-500, 500), 100);
                Object->SetActorLocation(NewLocation, false, nullptr, ETeleportType::ResetPhysics);

                Object->SetActorHiddenInGame(false);
                Object->SetActorEnableCollision(true);
                Object->SetActorTickEnabled(true);

                UE_LOG(LogTemp, Warning, TEXT("Pooled Object Reused at: %s"), *Object->GetActorLocation().ToString());

                return Object;
            }
        }
    }

    // 기존 풀에 없는 경우 새로 생성
    if (UWorld* World = GetWorld())
    {
        AActor* NewObject = World->SpawnActor<AActor>(ObjectClass);
        if (NewObject)
        {
            ObjectPool.FindOrAdd(ObjectClass).Add(NewObject);
            return NewObject;
        }
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
        Object->SetActorLocation(FVector::ZeroVector); // 기본 위치로 초기화
        Object->SetActorRotation(FRotator::ZeroRotator); // 회전 초기화
    }
}
