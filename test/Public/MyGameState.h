#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Engine/DataTable.h"
#include "TimerManager.h"
#include "ObjectPoolManager.h"
#include "Spike.h"
#include "Explosion.h"
#include "SpawnVolume.h"
#include "ImpedimentsVolume.h"
#include "MyGameState.generated.h"

class AObjectPoolManager;

USTRUCT(BlueprintType)
struct FImpedimentSpawnRow : public FTableRowBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ImpedimentClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ImpedimentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnChance;

};

UCLASS()
class TEST_API AMyGameState : public AGameState {
    GENERATED_BODY()

public:
    AMyGameState();

    virtual void BeginPlay() override;

    // ������ ��ȯ��
    int32 GetItemSpawnAmountForWave(int32 WaveIndex);

    // ���ع� ��ȯ��
    int32 GetImpedimentSpawnAmountForWave(int32 WaveIndex);

    // ���ع� ��ȯ �ֱ�
    float GetImpedimentSpawnIntervalForWave(int32 WaveIndex);

    // ������ ���� ����
    FItemSpawnRow* GetRandomItemFromTable();

    // ���ع� ��ȯ
    AActor* SpawnRandomImpediment(int32 WaveIndex);

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
    int32 Score;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
    int32 SpawnedCoinCount;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
    int32 CollectedCoinCount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
    float LevelDuration;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
    int32 CurrentLevelIndex;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
    int32 MaxLevels;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
    TArray<FName> LevelMapNames;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
    int32 CurrentWaveIndex;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
    int32 MaxWaves;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impediments")
    UDataTable* ImpedimentDataTable;

    // ���̺꺰 ���ع� ��ȯ���� �ֱ� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<int32> ImpedimentSpawnAmounts; // ���ع� ��ȯ��

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<float> ImpedimentSpawnIntervals; // ���ع� ��ȯ �ֱ�

    FTimerHandle LevelTimerHandle;
    FTimerHandle HUDUpdateTimerHandle;

    UFUNCTION(BlueprintPure, Category = "Score")
    int32 GetScore() const;

    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Level")
    void OnGameOver();

    UPROPERTY()
    AObjectPoolManager* PoolManager;

    void StartLevel();
    void StartWave();
    void EndWave();
    void OnLevelTimeUp();
    void OnCoinCollected();
    void EndLevel();
    void UpdateHUD();
};
