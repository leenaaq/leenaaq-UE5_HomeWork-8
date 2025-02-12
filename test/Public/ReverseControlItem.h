#pragma once

#include "CoreMinimal.h"
#include "DebuffBaseItem.h"
#include "ReverseControlItem.generated.h"

UCLASS()
class TEST_API AReverseControlItem : public ADebuffBaseItem
{
	GENERATED_BODY()

public:
	AReverseControlItem();

protected:
	virtual void ApplyDebuff(AActor* AffectedActor) override;
	virtual void RemoveDebuff(AActor* AffectedActor) override;

private:
	UPROPERTY()
	class APlayerController* PlayerController;
};
