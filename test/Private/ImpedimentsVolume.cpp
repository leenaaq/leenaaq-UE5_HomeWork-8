#include "ImpedimentsVolume.h"
#include "ObjectPoolManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "ItemSpawnRow.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

AImpedimentsVolume::AImpedimentsVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	SpawnInterval = 3.0f; // 기본 스폰 주기 (3초)
}

void AImpedimentsVolume::BeginPlay()
{
	Super::BeginPlay();

	if (!ImpedimentsDataTable)
	{
		//UE_LOG(LogTemp, Error, TEXT("No ImpedimentsDataTable"));
		return;
	}

	StartSpawning();
}


void AImpedimentsVolume::StartSpawning()
{
	if (SpawnAmount <= 0)
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AImpedimentsVolume::SpawnRandomImpedimentWithTimer,
		SpawnInterval,
		true
	);
}

void AImpedimentsVolume::StopSpawning()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

void AImpedimentsVolume::SetSpawnInterval(float Interval)
{
	SpawnInterval = Interval;

	if (SpawnAmount <= 0)
	{
		StopSpawning();
		return;
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(SpawnTimerHandle))
	{
		StopSpawning();
		StartSpawning();
	}
}

void AImpedimentsVolume::SpawnRandomImpedimentWithTimer()
{
	SpawnRandomImpediment();
}

AActor* AImpedimentsVolume::SpawnRandomImpediment()
{
	if (SpawnAmount <= 0)
	{
		return nullptr;
	}

	if (FItemSpawnRow* SelectedRow = GetRandomImpediment())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnImpediment(ActualClass);
		}
	}
	return nullptr;
}

FItemSpawnRow* AImpedimentsVolume::GetRandomImpediment() const
{
	if (!ImpedimentsDataTable) return nullptr;

	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ImpedimentsDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
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

FVector AImpedimentsVolume::GetRandomPointInVolume() const
{
	if (!SpawningBox) return FVector::ZeroVector;

	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();
	FTransform BoxTransform = SpawningBox->GetComponentTransform();

	FVector LocalRandomPoint = FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);

	FVector WorldRandomPoint = BoxTransform.TransformPosition(LocalRandomPoint);

	UE_LOG(LogTemp, Warning, TEXT("SpawningBox Location: %s, Random Spawn Location: %s"),
		*BoxOrigin.ToString(), *WorldRandomPoint.ToString());

	return WorldRandomPoint;
}




AActor* AImpedimentsVolume::SpawnImpediment(TSubclassOf<AActor> InImpedimentClass)
{
	if (!InImpedimentClass) return nullptr;

	this->PoolManager = Cast<AObjectPoolManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AObjectPoolManager::StaticClass())
	);

	if (this->PoolManager)
	{
		AActor* SpawnedActor = this->PoolManager->GetPooledObject(InImpedimentClass);
		if (SpawnedActor)
		{
			FVector SpawnLocation = GetRandomPointInVolume();

			// 장애물 위치 강제 설정
			SpawnedActor->SetActorHiddenInGame(false);
			SpawnedActor->SetActorEnableCollision(true);
			SpawnedActor->SetActorLocation(SpawnLocation, false, nullptr, ETeleportType::ResetPhysics);

			if (ASpike* Spike = Cast<ASpike>(SpawnedActor))
			{
				Spike->ResetSpike();
				Spike->ActivateSpike();
			}

			UE_LOG(LogTemp, Warning, TEXT("Spawn : %s"), *SpawnedActor->GetActorLocation().ToString());

			return SpawnedActor;
		}
	}

	return nullptr;
}








AActor* AImpedimentsVolume::SpawnSpecificImpediment(const FString& ImpedimentName)
{
	if (!ImpedimentsDataTable) return nullptr;

	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ImpedimentSpawnContext"));
	ImpedimentsDataTable->GetAllRows(ContextString, AllRows);

	// 특정 방해물 필터링 (ItemName을 사용)
	TArray<FItemSpawnRow*> FilteredRows;
	for (FItemSpawnRow* Row : AllRows)
	{
		if (Row && Row->ItemName.ToString() == ImpedimentName)  // 이름으로 비교
		{
			FilteredRows.Add(Row);
		}
	}

	if (FilteredRows.IsEmpty()) return nullptr;  // 필터링된 데이터가 없으면 리턴

	// 확률 기반 랜덤 선택
	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : FilteredRows)
	{
		TotalChance += Row->SpawnChance;
	}

	float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;

	for (FItemSpawnRow* Row : FilteredRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return SpawnImpediment(Row->ItemClass);
		}
	}

	return nullptr;
}
