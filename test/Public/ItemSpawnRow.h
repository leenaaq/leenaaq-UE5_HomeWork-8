#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "ItemSpawnRow.generated.h"

USTRUCT(BlueprintType)
struct FItemSpawnRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ImpedimentClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ImpedimentType = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnAmount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnInterval = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WaveIndex = 0;
};
