#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "DebuffBaseItem.generated.h"

UCLASS()
class TEST_API ADebuffBaseItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ADebuffBaseItem();

protected:
	virtual void ActivateItem(AActor* Activator) override;

	/** 디버프 지속 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debuff")
	float DebuffDuration;

	/** 디버프 적용 */
	virtual void ApplyDebuff(AActor* AffectedActor) PURE_VIRTUAL(ADebuffBaseItem::ApplyDebuff, );

	/** 디버프 해제 */
	virtual void RemoveDebuff(AActor* AffectedActor) PURE_VIRTUAL(ADebuffBaseItem::RemoveDebuff, );
};
