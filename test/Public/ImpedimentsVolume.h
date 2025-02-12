#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spike.h"
#include "ItemSpawnRow.h"
#include "ImpedimentsVolume.generated.h"

class UBoxComponent;
class USceneComponent;
struct FItemSpawnRow;
class AObjectPoolManager;

UCLASS()
class TEST_API AImpedimentsVolume : public AActor
{
	GENERATED_BODY()

private:
	AObjectPoolManager* PoolManager;

	TSubclassOf<AActor> ImpedimentClass;

public:
	AImpedimentsVolume();
	void DestroyImpediment();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StartSpawning();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StopSpawning();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetSpawnInterval(float Interval);

	void SpawnRandomImpedimentWithTimer();

	AActor* SpawnRandomImpediment();

	AActor* SpawnSpecificImpediment(const FString& ImpedimentType);

	AActor* SpawnImpediment(TSubclassOf<AActor> ImpedimentClass);

	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 SpawnAmount;

protected:
	virtual void BeginPlay() override;	

	FItemSpawnRow* GetRandomImpediment() const;

	FVector GetRandomPointInVolume() const;

	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ImpedimentsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float SpawnInterval;

};
