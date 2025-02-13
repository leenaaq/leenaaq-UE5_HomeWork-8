

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor1.generated.h"

UCLASS()
class TEST_API ATestActor1 : public AActor
{
	GENERATED_BODY()

public:
	ATestActor1();

protected:
	virtual void BeginPlay() override;

	void MovePlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComp2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float MaxHeight;

private:
	FTimerHandle MoveTimerHandle;
	bool bMovingUp;
	float InitialZ;
};
