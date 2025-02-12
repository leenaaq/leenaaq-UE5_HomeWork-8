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
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ImpedimentClass; // 소환할 방해물 클래스

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ImpedimentType; // 종류

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnChance; // 확률

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SpawnAmount; // 소환량

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnInterval; // 소환 주기

};
 