#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class TEST_API IItemInterface
{
	GENERATED_BODY()

public:
	// �÷��̾ ������ �������� �� ȣ��
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp, // ������ �߻��� �ڱ� �ڽ� (SphereComp)
		AActor* OtherActor, // �ε��� ��� ���� (�÷��̾�)
		UPrimitiveComponent* OtherComp, // �÷��̾ �پ��ִ� ĸ��?
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;

	// �÷��̾ ������ ��Ż���� �� ȣ��
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;

    // �������� ���Ǿ��� �� ȣ��
    virtual void ActivateItem(AActor* Activator) = 0;
    // ������ ���� ��ȯ
    virtual FName GetItemType() const = 0;

};
