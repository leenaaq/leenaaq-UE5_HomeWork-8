#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor5.generated.h"

UCLASS()
class TEST_API ATestActor5 : public AActor
{
	GENERATED_BODY()
	
public:	
	ATestActor5();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "name|Properties")
	float DisappearTime;

	FTimerHandle TimerHandle;

	void HidePlatform();
};
