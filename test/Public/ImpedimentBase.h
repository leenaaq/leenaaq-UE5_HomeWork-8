#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class USphereComponent;
class UStaticMeshComponent;
class USoundBase;
class USoundAttenuation;
struct FHitResult;

#include "ImpedimentBase.generated.h"

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
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void PlayOverlapSound(AActor* TargetActor);

	UFUNCTION()
	void PlayActionSound();

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* SpawnSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundAttenuation* SoundAttenuation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* OverlapSound = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Impediment")
	USceneComponent* SceneRoot = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Impediment")
	USphereComponent* CollisionComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Impediment")
	UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impediment")
	float DamageAmount = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impediment")
	float Lifetime = 5.0f;

	UPROPERTY(Transient)
	FTimerHandle DestroyTimerHandle;
};
