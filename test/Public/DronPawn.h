#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DronPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCapsuleComponent;
class USkeletalComponent;
struct FInputActionValue;
class UFloatingPawnMovement;
class UBoxComponent;

UCLASS()
class TEST_API ADronPawn : public APawn
{
	GENERATED_BODY()

public:
	ADronPawn();
protected:

	float DronSpeed;

	// 캡슐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;

	// 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComp;

	// 스프링암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UFloatingPawnMovement* FloatingMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* FootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	bool bCanMoveDown;

	UFUNCTION()
	void OnFootOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnFootEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);



public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	void DronMove(const FInputActionValue& value);

	void DronLook(const FInputActionValue& value);

	void DronRotation(const FInputActionValue& value);

	bool CheckIfOnGround();
};
