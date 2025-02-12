#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

// 스파르타 로그 추가, 워닝 이상만 출력,  All은 필요하면 모든 나중에 모든 로그를 활성화 할 수 있음
DECLARE_LOG_CATEGORY_EXTERN(LogSparta, Warning, All);


UCLASS()
class TEST_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Properties")
	float RotationSpeed;

	virtual void BeginPlay() override;
	//virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	//virtual void Destroyed() override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Item|Actions")
	void ResetActorPosition();

	UFUNCTION(BlueprintPure, Category = "Item|Properties")
	float GetRotationSpeed() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Item|Event")
	void OnItemPickUP();
};
