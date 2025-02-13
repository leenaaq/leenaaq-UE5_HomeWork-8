#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Engine/DataTable.h"

class AObjectPoolManager;
class ASpawnVolume;
class AImpedimentsVolume;
class AActor;
class AExplosion;
struct FItemSpawnRow;

#include "MyGameState.generated.h"

USTRUCT(BlueprintType)
struct FImpedimentSpawnRow : public FTableRowBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ImpedimentClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ImpedimentType = TEXT("");

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnChance = 0.0f;
};

UCLASS()
class TEST_API AMyGameState : public AGameState {
    GENERATED_BODY()

public:
    AMyGameState();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
    int32 CurrentWaveIndex = 0;

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintPure, Category = "Score")
    int32 GetScore() const;

    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Level")
    void OnGameOver();

    UFUNCTION(BlueprintCallable, Category = "Wave")
    void AdvanceWave();

    UFUNCTION()
    void OnCoinCollected();

    UFUNCTION()
    void UpdateHUD();

    UFUNCTION(BlueprintPure, Category = "Wave")
    int32 GetCurrentWaveIndex() const { return CurrentWaveIndex; }

protected:
    void StartLevel();
    void StartWave();
    void EndWave();
    void OnLevelTimeUp();
    void EndLevel();

    int32 GetItemSpawnAmountForWave(int32 WaveIndex);
    int32 GetImpedimentSpawnAmountForWave(int32 WaveIndex);
    float GetImpedimentSpawnIntervalForWave(int32 WaveIndex);
    FItemSpawnRow* GetRandomItemFromTable();
    AActor* SpawnRandomImpediment(int32 WaveIndex);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
    int32 Score = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
    int32 SpawnedCoinCount = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
    int32 CollectedCoinCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
    float LevelDuration = 30.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
    int32 CurrentLevelIndex = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
    int32 MaxLevels = 3;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
    TArray<FName> LevelMapNames;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
    int32 MaxWaves = 3;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impediments")
    UDataTable* ImpedimentDataTable = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<int32> ImpedimentSpawnAmounts{ 0, 10, 15 };

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<float> ImpedimentSpawnIntervals{ 0.0f, 1.5f, 2.0f };

    UPROPERTY(Transient)
    FTimerHandle LevelTimerHandle;

    UPROPERTY(Transient)
    FTimerHandle HUDUpdateTimerHandle;

    UPROPERTY(Transient)
    AObjectPoolManager* PoolManager = nullptr;
};
