#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor1_2.generated.h"

UCLASS()
class TEST_API ATestActor1_2 : public AActor
{
	GENERATED_BODY()

public:
	ATestActor1_2();

protected:
	virtual void BeginPlay() override;

	void RotatePlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComp2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float RotationSpeed;

private:
	FTimerHandle RotationTimerHandle;
};
