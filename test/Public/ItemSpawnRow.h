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
    TSubclassOf<AActor> ImpedimentClass; // ��ȯ�� ���ع� Ŭ����

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ImpedimentType; // ����

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnChance; // Ȯ��

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SpawnAmount; // ��ȯ��

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnInterval; // ��ȯ �ֱ�

};
 