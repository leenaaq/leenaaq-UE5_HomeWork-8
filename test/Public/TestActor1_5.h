#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInterface.h"
#include "TestActor1_5.generated.h"

UCLASS()
class TEST_API ATestActor1_5 : public AActor
{
	GENERATED_BODY()

public:
	ATestActor1_5();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnSphere();

	UFUNCTION()
	void ReturnSphere(UStaticMeshComponent* Sphere);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* SpawnPoint;

	UPROPERTY()
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, Category = "Properties")
	int32 PoolSize = 300;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float SpawnInterval = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float SphereSize = 30.0f;

	UPROPERTY()
	UStaticMesh* SphereMesh;

	UPROPERTY()
	UMaterialInterface* SphereMaterial;

	UPROPERTY(EditAnywhere, Category = "Physics")
	float Mass = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Physics")
	float LinearDamping = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Physics")
	float AngularDamping = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Physics")
	FVector LaunchVelocity = FVector(500.0f, 0.0f, 200.0f);

private:
	TArray<UStaticMeshComponent*> SpherePool;
	FTimerHandle SpawnTimerHandle;
};
