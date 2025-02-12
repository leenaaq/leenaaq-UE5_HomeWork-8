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

	/** ����� ���� �ð� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debuff")
	float DebuffDuration;

	/** ����� ���� */
	virtual void ApplyDebuff(AActor* AffectedActor) PURE_VIRTUAL(ADebuffBaseItem::ApplyDebuff, );

	/** ����� ���� */
	virtual void RemoveDebuff(AActor* AffectedActor) PURE_VIRTUAL(ADebuffBaseItem::RemoveDebuff, );
};
