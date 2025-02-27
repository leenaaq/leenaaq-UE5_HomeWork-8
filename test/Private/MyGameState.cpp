#include "MyGameState.h"
#include "MyGameInstance.h"
#include "TestPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "ImpedimentsVolume.h"
#include "CoinItem.h"
#include "Spike.h"
#include "Explosion.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

AMyGameState::AMyGameState()
{
}

void AMyGameState::BeginPlay()
{
    Super::BeginPlay();

    PoolManager = Cast<AObjectPoolManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AObjectPoolManager::StaticClass()));

    if (!PoolManager)
    {
        UE_LOG(LogTemp, Error, TEXT("PoolManager is NULL"));
        return;
    }

    StartLevel();

    GetWorldTimerManager().SetTimer(
        HUDUpdateTimerHandle,
        this,
        &AMyGameState::UpdateHUD,
        0.1f,
        true
    );
}

int32 AMyGameState::GetScore() const
{
    return Score;
}

void AMyGameState::AddScore(int32 Amount)
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
        if (MyGameInstance)
        {
            MyGameInstance->AddToScore(Amount);
        }
    }
}

void AMyGameState::StartLevel()
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (ATestPlayerController* TestPlayerController = Cast<ATestPlayerController>(PlayerController))
        {
            TestPlayerController->ShowGameHUD();
        }
    }

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
        if (MyGameInstance)
        {
            CurrentLevelIndex = MyGameInstance->CurrentLevelIndex;
        }
    }

    this->CurrentWaveIndex = 0;
    StartWave();

    GetWorldTimerManager().SetTimer(
        LevelTimerHandle,
        this,
        &AMyGameState::OnLevelTimeUp,
        LevelDuration,
        false
    );
}

void AMyGameState::StartWave()
{
    UE_LOG(LogTemp, Warning, TEXT("Wave: %d Start"), CurrentWaveIndex);

    SpawnedCoinCount = 0;
    CollectedCoinCount = 0;

    TArray<AActor*> FoundVolumes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

    TArray<AActor*> FoundImpediments;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AImpedimentsVolume::StaticClass(), FoundImpediments);

    if (FoundVolumes.Num() == 0) return;

    int32 ItemToSpawn = GetItemSpawnAmountForWave(CurrentWaveIndex);
    int32 ImpedimentToSpawn = GetImpedimentSpawnAmountForWave(CurrentWaveIndex);
    float ImpedimentSpawnInterval = GetImpedimentSpawnIntervalForWave(CurrentWaveIndex);

    for (int32 i = 0; i < ItemToSpawn; i++)
    {
        for (AActor* Volume : FoundVolumes)
        {
            ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(Volume);
            if (SpawnVolume) {
                AActor* SpawnedItem = SpawnVolume->SpawnRandomItem();
                if (SpawnedItem && SpawnedItem->IsA(ACoinItem::StaticClass()))
                {
                    SpawnedCoinCount++;
                }
            }
        }
    }

    for (AActor* Volume : FoundImpediments) {
        AImpedimentsVolume* ImpedimentVolume = Cast<AImpedimentsVolume>(Volume);
        if (ImpedimentVolume)
        {
            ImpedimentVolume->SpawnAmount = ImpedimentToSpawn;
            ImpedimentVolume->SetSpawnInterval(ImpedimentSpawnInterval);
            ImpedimentVolume->CurrentWaveIndex = CurrentWaveIndex;

            if (ImpedimentToSpawn > 0)
            {
                ImpedimentVolume->StartSpawning();
            }
            else
            {
                ImpedimentVolume->StopSpawning();
            }
        }
    }
}

int32 AMyGameState::GetItemSpawnAmountForWave(int32 WaveIndex)
{
    if (WaveIndex == 1) return 20;
    else if (WaveIndex == 2) return 30;
    else return 40;
}

int32 AMyGameState::GetImpedimentSpawnAmountForWave(int32 WaveIndex)
{
    if (ImpedimentSpawnAmounts.IsValidIndex(WaveIndex))
    {
        return ImpedimentSpawnAmounts[WaveIndex];
    }
    return 0;
}

float AMyGameState::GetImpedimentSpawnIntervalForWave(int32 WaveIndex)
{
    if (ImpedimentSpawnIntervals.IsValidIndex(WaveIndex))
    {
        return ImpedimentSpawnIntervals[WaveIndex];
    }
    return 0.0f;
}

FItemSpawnRow* AMyGameState::GetRandomItemFromTable()
{
    if (!ImpedimentDataTable) return nullptr;

    TArray<FItemSpawnRow*> AllRows;
    static const FString ContextString(TEXT("ItemSpawnContext"));
    ImpedimentDataTable->GetAllRows(ContextString, AllRows);

    if (AllRows.IsEmpty()) return nullptr;

    float TotalChance = 0.0f;
    for (const FItemSpawnRow* Row : AllRows)
    {
        if (Row) TotalChance += Row->SpawnChance;
    }

    const float RandValue = FMath::FRandRange(0.0f, TotalChance);
    float AccumulateChance = 0.0f;

    for (FItemSpawnRow* Row : AllRows)
    {
        AccumulateChance += Row->SpawnChance;
        if (RandValue <= AccumulateChance)
        {
            return Row;
        }
    }

    return nullptr;
}

void AMyGameState::EndWave()
{
    EndLevel();
}

void AMyGameState::OnLevelTimeUp()
{
    OnGameOver();
}

void AMyGameState::OnCoinCollected()
{
    CollectedCoinCount++;

    if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
    {
        if (CurrentWaveIndex + 1 < MaxWaves)
        {
            AdvanceWave();
            StartWave();

            float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
            GetWorldTimerManager().SetTimer(
                LevelTimerHandle,
                this,
                &AMyGameState::OnLevelTimeUp,
                RemainingTime + 30.0f,
                false
            );
        }
        else
        {
            EndWave();
        }
    }
}


void AMyGameState::EndLevel()
{
    GetWorldTimerManager().ClearTimer(LevelTimerHandle);

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
        if (MyGameInstance)
        {
            AddScore(Score);
            CurrentLevelIndex++;
            MyGameInstance->CurrentLevelIndex = CurrentLevelIndex;
        }
    }

    if (CurrentLevelIndex >= MaxLevels)
    {
        OnGameOver();
        return;
    }

    if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
    {
        UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
    }
    else
    {
        OnGameOver();
    }
}

void AMyGameState::OnGameOver()
{
    GetWorldTimerManager().ClearTimer(LevelTimerHandle);
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (ATestPlayerController* TestPlayerController = Cast<ATestPlayerController>(PlayerController))
        {
            TestPlayerController->SetPause(true);
            TestPlayerController->ShowMainMenu(true);
        }
    }
}

void AMyGameState::UpdateHUD()
{
    AsyncTask(ENamedThreads::GameThread, [this]()
        {
        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            ATestPlayerController* TestPlayerController = Cast<ATestPlayerController>(PlayerController);
            if (TestPlayerController && TestPlayerController->GetHUDWidget())
            {
                UUserWidget* HUDWidget = TestPlayerController->GetHUDWidget();

                if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
                {
                    float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
                    TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));

                    if (FMath::FloorToInt(RemainingTime) <= 10)
                    {
                        UFunction* PlayAnimFunc = HUDWidget->FindFunction(FName("PlayTimeAnim"));
                        if (PlayAnimFunc)
                        {
                            HUDWidget->ProcessEvent(PlayAnimFunc, nullptr);
                        }
                    }
                }

                if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
                {
                    if (UGameInstance* GameInstance = GetGameInstance())
                    {
                        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
                        if (MyGameInstance)
                        {
                            ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), MyGameInstance->TotalScore)));
                        }
                    }
                }

                if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
                {
                    LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
                }

                if (UTextBlock* CurrentCoinText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("CurrentCoin"))))
                {
                    CurrentCoinText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CollectedCoinCount, SpawnedCoinCount)));
                }

                if (UTextBlock* WaveText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
                {
                    WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex + 1)));
                }
            }
        }
        });
}


void AMyGameState::AdvanceWave()
{
    if (this->CurrentWaveIndex + 1 < MaxWaves)
    {
        this->CurrentWaveIndex++;
    }
}