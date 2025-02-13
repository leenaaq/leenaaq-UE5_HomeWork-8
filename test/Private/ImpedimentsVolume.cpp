#include "ImpedimentsVolume.h"
#include "ObjectPoolManager.h"
#include "Kismet/GameplayStatics.h"
#include "ItemSpawnRow.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "MyGameState.h"
#include "ItemSpawnRow.h"
#include "Explosion.h"
#include "MyGameState.h"
#include "Spike.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

AImpedimentsVolume::AImpedimentsVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	SpawnInterval = 3.0f;
	PoolManager = nullptr;
}

void AImpedimentsVolume::BeginPlay()
{
	Super::BeginPlay();

	PoolManager = Cast<AObjectPoolManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AObjectPoolManager::StaticClass()));
	if (!PoolManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("No existing PoolManager found, spawning a new one."));
		PoolManager = GetWorld()->SpawnActor<AObjectPoolManager>();

		if (!PoolManager)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn PoolManager!"));
			return;
		}
	}

	if (!ImpedimentsDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("No ImpedimentsDataTable"));
		return;
	}

	AMyGameState* GameState = Cast<AMyGameState>(GetWorld()->GetGameState<AMyGameState>());
	if (GameState)
	{
		CurrentWaveIndex = GameState->CurrentWaveIndex;
		UE_LOG(LogTemp, Warning, TEXT("AImpedimentsVolume - Loaded Wave Index: %d"), CurrentWaveIndex);
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

	GetWorld()->GetTimerManager().SetTimer(
		ExplosionSpawnTimerHandle,
		this,
		&AImpedimentsVolume::SpawnExplosion,
		ExplosionSpawnInterval,
		true
	);
}

void AImpedimentsVolume::SpawnExplosion()
{
	if (!PoolManager)
	{
		UE_LOG(LogTemp, Error, TEXT("PoolManager is NULL in SpawnExplosion()"));
		return;
	}

	AExplosion* Explosion = Cast<AExplosion>(PoolManager->GetPooledObject(AExplosion::StaticClass()));
	if (Explosion)
	{
		Explosion->SetActorLocation(GetActorLocation());
		Explosion->ActivateExplosion();
	}
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
	if (!ImpedimentsDataTable) {
		UE_LOG(LogTemp, Error, TEXT("ImpedimentsVolumeTable is not set."));
		return nullptr;
	}

	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ImpedimentSpawnContext"));
	ImpedimentsDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) {
		UE_LOG(LogTemp, Warning, TEXT("No impediments are registered in ImpedimentsVolumeTable."));
		return nullptr;
	}

	TArray<FItemSpawnRow*> FilteredRows;
	for (FItemSpawnRow* Row : AllRows) {
		UE_LOG(LogTemp, Warning, TEXT("Checking Row: WaveIndex = %d, CurrentWaveIndex = %d"), Row->WaveIndex, CurrentWaveIndex);
		if (Row && Row->WaveIndex == CurrentWaveIndex) {
			FilteredRows.Add(Row);
		}
	}

	if (FilteredRows.IsEmpty()) {
		UE_LOG(LogTemp, Warning, TEXT("No impediments found for Wave %d."), CurrentWaveIndex);
		return nullptr;
	}

	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : FilteredRows) {
		TotalChance += Row->SpawnChance;
	}

	if (TotalChance != 100.0f) {
		UE_LOG(LogTemp, Error, TEXT("Total spawn chance in ImpedimentsVolumeTable for Wave %d is not 100. Spawning aborted."), CurrentWaveIndex);
		return nullptr;
	}

	float RandValue = FMath::FRandRange(0.0f, 100.0f);
	float AccumulateChance = 0.0f;

	for (FItemSpawnRow* Row : FilteredRows) {
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance) {
			UE_LOG(LogTemp, Warning, TEXT("Spawning impediment: %s"), *Row->ImpedimentClass->GetName());

			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(Row->ImpedimentClass, GetRandomPointInVolume(), FRotator::ZeroRotator);
			if (!SpawnedActor) {
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn impediment: %s"), *Row->ImpedimentClass->GetName());
				return nullptr;
			}

			if (ASpike* SpawnedSpike = Cast<ASpike>(SpawnedActor)) {
				UE_LOG(LogTemp, Warning, TEXT("Spike spawned and activated"));
				SpawnedSpike->ActivateSpike();
			}
			else if (AExplosion* SpawnedExplosion = Cast<AExplosion>(SpawnedActor)) {
				UE_LOG(LogTemp, Warning, TEXT("Explosion spawned and activated"));
				SpawnedExplosion->ActivateExplosion();
			}

			return SpawnedActor;
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
	if (!SpawningBox) {
		UE_LOG(LogTemp, Error, TEXT("SpawningBox is NULL, cannot determine spawn location."));
		return FVector::ZeroVector;
	}

	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->Bounds.Origin;

	FVector SpawnLocation = BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		BoxOrigin.Z
	);

	UE_LOG(LogTemp, Warning, TEXT("Generated Spawn Location: %s"), *SpawnLocation.ToString());

	return SpawnLocation;
}




AActor* AImpedimentsVolume::SpawnImpediment(TSubclassOf<AActor> ImpedimentClass)
{
	if (!PoolManager)
	{
		UE_LOG(LogTemp, Error, TEXT("PoolManager is NULL"));
		return nullptr;
	}

	if (!ImpedimentClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid ImpedimentClass"));
		return nullptr;
	}

	FVector SpawnLocation = this->GetActorLocation();
	SpawnLocation.Z += 50.0f;

	AActor* SpawnedImpediment = PoolManager->GetPooledObject(ImpedimentClass);
	if (SpawnedImpediment)
	{
		SpawnedImpediment->SetActorLocation(SpawnLocation);
		SpawnedImpediment->SetActorHiddenInGame(false);
		SpawnedImpediment->SetActorEnableCollision(true);
		return SpawnedImpediment;
	}

	UE_LOG(LogTemp, Error, TEXT("Failed to spawn Impediment"));
	return nullptr;
}





AActor* AImpedimentsVolume::SpawnSpecificImpediment(const FString& ImpedimentName)
{
	if (!ImpedimentsDataTable) return nullptr;

	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ImpedimentSpawnContext"));
	ImpedimentsDataTable->GetAllRows(ContextString, AllRows);

	TArray<FItemSpawnRow*> FilteredRows;
	for (FItemSpawnRow* Row : AllRows)
	{
		if (Row && Row->ItemName.ToString() == ImpedimentName)
		{
			FilteredRows.Add(Row);
		}
	}

	if (FilteredRows.IsEmpty()) return nullptr;

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
