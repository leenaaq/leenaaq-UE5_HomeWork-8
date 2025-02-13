#pragma once

#include "CoreMinimal.h"
#include "ImpedimentBase.h"
#include "Explosion.generated.h"

class UParticleSystem;
class UPrimitiveComponent;
struct FHitResult;

UCLASS()
class TEST_API AExplosion : public AImpedimentBase
{
    GENERATED_BODY()

public:
    AExplosion();

    UFUNCTION(BlueprintCallable, Category = "Explosion")
    void StartExplosion();

    UFUNCTION(BlueprintCallable, Category = "Explosion")
    void ActivateExplosion();

    void DeactivateExplosion();

protected:
    virtual void BeginPlay() override;
    virtual void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult) override;

private:
    UPROPERTY(EditAnywhere, Category = "Explosion")
    float ExplosionInterval = 3.0f;

    UPROPERTY(EditAnywhere, Category = "Explosion")
    float ExplosionRadius = 300.0f;

    UPROPERTY(EditAnywhere, Category = "Explosion")
    float ExplosionDamage = 50.0f;

    UPROPERTY(EditAnywhere, Category = "Explosion")
    float ExplosionDuration = 0.7f;

    UPROPERTY(EditAnywhere, Category = "Explosion")
    UParticleSystem* ExplosionParticle = nullptr;

    UPROPERTY(Transient)
    FTimerHandle ExplosionTimerHandle;

    UPROPERTY(Transient)
    FTimerHandle ExplosionDeactivateTimerHandle;
};
