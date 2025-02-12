#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ObjectPoolManager.h"
#include "GameFramework/Actor.h"
#include "ImpedimentBase.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class TEST_API AImpedimentBase : public AActor
{
	GENERATED_BODY()

public:
	AImpedimentBase();
	virtual void DestroyImpediment();
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Impediment")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Impediment")
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Impediment")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impediment")
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impediment")
	float Lifetime;

	FTimerHandle DestroyTimerHandle;
};
