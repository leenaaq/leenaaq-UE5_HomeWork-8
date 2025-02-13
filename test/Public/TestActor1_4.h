#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor1_4.generated.h"

UCLASS()
class TEST_API ATestActor1_4 : public AActor
{
	GENERATED_BODY()

public:
	ATestActor1_4();

protected:
	virtual void BeginPlay() override;

	void RotateComponents();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComp1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComp2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float RotationSpeed = 2.0f;

private:
	FTimerHandle RotationTimerHandle;
	float TargetRotation1;
	float TargetRotation2;
	bool bRotatingToTarget;
};
