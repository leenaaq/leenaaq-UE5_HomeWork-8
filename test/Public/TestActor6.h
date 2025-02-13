#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor6.generated.h"

UCLASS()
class TEST_API ATestActor6 : public AActor
{
	GENERATED_BODY()

public:
	ATestActor6();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "name|Properties")
	float DisappearTime;

	FTimerHandle TimerHandle;

	FVector CurrentLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "name|Properties")
	FVector MinRange; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "name|Properties")
	FVector MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "name|Properties")
	float RandomLocationTime;

	void RandomLocation();
};
