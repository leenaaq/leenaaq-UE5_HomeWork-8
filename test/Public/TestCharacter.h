#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "TestCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;

UCLASS()
class TEST_API ATestCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATestCharacter();

    UFUNCTION(BlueprintCallable)
    void ShowSnailIcon();

    UFUNCTION(BlueprintCallable)
    void HideSnailIcon();

    UFUNCTION(BlueprintCallable)
    void ShowArrowIcon();

    UFUNCTION(BlueprintCallable)
    void HideArrowIcon();

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealth() const;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void AddHealth(float Amount);

    void SetReverseControl(bool bShouldReverse);
    void SetSpeedMultiplier(float Multiplier, bool bDisableSprint);
    void RemoveDebuff();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual float TakeDamage(
        float DamageAmount,
        struct FDamageEvent const& DamageEvent,
        AController* EventInstrigator,
        AActor* DamageCauser) override;

    UFUNCTION()
    void Move(const FInputActionValue& value);

    UFUNCTION()
    void StartJump(const FInputActionValue& value);

    UFUNCTION()
    void StopJump(const FInputActionValue& value);

    UFUNCTION()
    void Look(const FInputActionValue& value);

    UFUNCTION()
    void StartSprint(const FInputActionValue& value);

    UFUNCTION()
    void StopSprint(const FInputActionValue& value);

    void OnDeath();
    void UpdateOverheadHP();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float Health = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComp;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* OverheadWidget;

private:
    UPROPERTY(EditAnywhere, Category = "Movement")
    float NormalSpeed = 800.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float SprintSpeedMultiplier = 1.5f;

    UPROPERTY(VisibleAnywhere, Category = "Movement")
    float SprintSpeed = 1200.0f;

    UPROPERTY(VisibleAnywhere, Category = "Movement")
    float OriginalSpeed = 800.0f;

    UPROPERTY(VisibleAnywhere, Category = "State")
    bool bIsControlReversed = false;

    UPROPERTY(VisibleAnywhere, Category = "State")
    bool bCanSprint = true;
};
