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
	// 플레이어가 범위에 진입했을 때 호출
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp, // 오버랩 발생한 자기 자신 (SphereComp)
		AActor* OtherActor, // 부딪힌 상대 액터 (플레이어)
		UPrimitiveComponent* OtherComp, // 플레이어에 붙어있는 캡슐?
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;

	// 플레이어가 범위를 이탈했을 때 호출
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;

    // 아이템이 사용되었을 때 호출
    virtual void ActivateItem(AActor* Activator) = 0;
    // 아이템 유형 반환
    virtual FName GetItemType() const = 0;

};
