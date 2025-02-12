#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor4.generated.h"

UCLASS()
class TEST_API ATestActor4 : public AActor
{
	GENERATED_BODY()

public:
	ATestActor4();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "name|Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "name|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "name|Properties")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "name|Properties")
	float MaxHeight;


	FVector CurrentLocation;



private:
	void CheckAndResetHeight();
};
