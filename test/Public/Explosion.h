#pragma once

#include "CoreMinimal.h"
#include "ImpedimentBase.h"
#include "Explosion.generated.h"

UCLASS()
class TEST_API AExplosion : public AImpedimentBase
{
	GENERATED_BODY()

public:
	AExplosion();

protected:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;
};
