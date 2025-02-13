#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor2.generated.h"

UCLASS()
class TEST_API ATestActor2 : public AActor
{
	GENERATED_BODY()
	
public:	
	ATestActor2();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "name|Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "name|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "name|Properties")
	float RotationSpeed;
};
