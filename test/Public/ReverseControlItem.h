#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"

#include "ReverseControlItem.generated.h"

UCLASS()
class TEST_API AReverseControlItem : public ABaseItem
{
    GENERATED_BODY()

public:
    AReverseControlItem();

protected:
    virtual void ActivateItem(AActor* Activator) override;

private:
    UPROPERTY(EditAnywhere, Category = "Debuff")
    float DebuffDuration = 2.0f;
};
