#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "ObjectPoolManager.h"
#include "Components/BoxComponent.h"
#include "ImpedimentsVolume.generated.h"

class UBoxComponent;
class USceneComponent;
struct FItemSpawnRow;
class AObjectPoolManager;
class AExplosion;

UCLASS()
class TEST_API AImpedimentsVolume : public AActor
{
    GENERATED_BODY()

private:
    AObjectPoolManager* PoolManager;

public:
    AImpedimentsVolume();

    AActor* SpawnImpediment(TSubclassOf<AActor> ImpedimentClass);

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void StartSpawning();

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void StopSpawning();

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SetSpawnInterval(float Interval);

    void SpawnRandomImpedimentWithTimer();
    AActor* SpawnRandomImpediment();
    AActor* SpawnSpecificImpediment(const FString& ImpedimentType);

    FTimerHandle SpawnTimerHandle;
    FTimerHandle ExplosionSpawnTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    int32 SpawnAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    int32 CurrentWaveIndex;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    UDataTable* ImpedimentsDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float SpawnInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float ExplosionSpawnInterval;

protected:
    virtual void BeginPlay() override;
    FItemSpawnRow* GetRandomImpediment() const;
    FVector GetRandomPointInVolume() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;

private:
    UFUNCTION()
    void SpawnExplosion();
};
