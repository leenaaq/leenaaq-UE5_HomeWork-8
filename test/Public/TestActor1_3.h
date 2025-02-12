#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "TestActor1_3.generated.h"

UCLASS()
class TEST_API ATestActor1_3 : public AActor
{
    GENERATED_BODY()

public:
    ATestActor1_3();

protected:
    virtual void BeginPlay() override;

    FTimerHandle DestroyTimerHandle;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void PlaySoundAndDestroy();

    UFUNCTION()
    void DestroyActor();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
    USceneComponent* SceneRoot;

    UPROPERTY(EditAnywhere, Category = "Components")
    UBoxComponent* TriggerBox;

    UPROPERTY(EditAnywhere, Category = "Components")
    UStaticMeshComponent* BaseMesh;

    UPROPERTY(EditAnywhere, Category = "Components")
    UStaticMeshComponent* DetailMesh;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* WoodBoomSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* DestroyEffect;
};
