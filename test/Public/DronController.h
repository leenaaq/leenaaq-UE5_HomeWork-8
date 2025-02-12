#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DronController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class TEST_API ADronController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADronController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DronMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DronLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DronRotationAction;

protected:
	virtual void BeginPlay() override;
};
