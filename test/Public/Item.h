#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

// ���ĸ�Ÿ �α� �߰�, ���� �̻� ���,  All�� �ʿ��ϸ� ��� ���߿� ��� �α׸� Ȱ��ȭ �� �� ����
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
