#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPoolManager.generated.h"

UCLASS()
class TEST_API AObjectPoolManager : public AActor
{
	GENERATED_BODY()

public:
	AObjectPoolManager();

	AActor* GetPooledObject(TSubclassOf<AActor> ObjectClass);
	void ReturnPooledObject(AActor* Object);

	UPROPERTY()
	AObjectPoolManager* PoolManager;

protected:
	virtual void BeginPlay() override;

private:
	TMap<TSubclassOf<AActor>, TArray<AActor*>> ObjectPool;
};
