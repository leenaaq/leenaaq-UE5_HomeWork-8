#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"

#include "SlowingItem.generated.h"

UCLASS()
class TEST_API ASlowingItem : public ABaseItem
{
    GENERATED_BODY()

public:
    ASlowingItem();

protected:
    virtual void ActivateItem(AActor* Activator) override;

private:
    UPROPERTY(EditAnywhere, Category = "Debuff")
    float DebuffDuration = 2.5f;
};
