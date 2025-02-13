#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "CoinItem.generated.h"

UCLASS()
class TEST_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ACoinItem();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationSpeed;

	virtual void BeginPlay() override;
	virtual void ActivateItem(AActor* Activator) override;
private:
	
	FTimerHandle RotationTimerHandle;
	float TargetYaw;
	void RotateItem();
};
