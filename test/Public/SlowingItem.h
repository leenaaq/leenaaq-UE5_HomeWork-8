#pragma once

#include "CoreMinimal.h"
#include "DebuffBaseItem.h"
#include "SlowingItem.generated.h"

UCLASS()
class TEST_API ASlowingItem : public ADebuffBaseItem
{
	GENERATED_BODY()

public:
	ASlowingItem();

protected:
	virtual void ApplyDebuff(AActor* AffectedActor) override;
	virtual void RemoveDebuff(AActor* AffectedActor) override;
};
