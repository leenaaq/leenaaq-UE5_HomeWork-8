#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor1_1.generated.h"

UCLASS()
class TEST_API ATestActor1_1 : public AActor
{
	GENERATED_BODY()

public:
	ATestActor1_1();

protected:
	virtual void BeginPlay() override;

	void MovePlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComp2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float MaxDistance;

private:
	FTimerHandle MoveTimerHandle;
	bool bMovingRight;
	float InitialX;
};
