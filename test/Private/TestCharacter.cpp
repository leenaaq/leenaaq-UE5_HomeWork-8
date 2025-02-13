#include "TestCharacter.h"
#include "TestPlayerController.h"
#include "MyGameState.h"
#include "EnhancedInputComponent.h"
#include "Components/Image.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

ATestCharacter::ATestCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();
}

void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ATestPlayerController* PlayerController = Cast<ATestPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ATestCharacter::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ATestCharacter::StartJump
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ATestCharacter::StopJump
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ATestCharacter::StartSprint
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ATestCharacter::StopSprint
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ATestCharacter::Look
				);
			}

			if (PlayerController->ChangeAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ChangeAction,
					ETriggerEvent::Triggered,
					this,
					&ATestCharacter::Look
				);
			}
		}
	}
}

void ATestCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	FVector2D MoveInput = value.Get<FVector2D>();

	if (bIsControlReversed)
	{
		MoveInput.X *= -1;
		MoveInput.Y *= -1;
	}

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ATestCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}
void ATestCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}
void ATestCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}
void ATestCharacter::StartSprint(const FInputActionValue& value)
{
	if (bCanSprint && GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}
void ATestCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

float ATestCharacter::GetHealth() const
{
	return Health;
}

void ATestCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
}

float ATestCharacter::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstrigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstrigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();

	if (Health <= 0.0f)
	{
		OnDeath();
	}
	return ActualDamage;
}

void ATestCharacter::OnDeath()
{
	AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (MyGameState)
	{
		MyGameState->OnGameOver();
	}
}

void ATestCharacter::UpdateOverheadHP()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}
}

void ATestCharacter::SetReverseControl(bool bShouldReverse)
{
	bIsControlReversed = bShouldReverse;
}

void ATestCharacter::SetSpeedMultiplier(float Multiplier, bool bDisableSprint)
{
	GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed * Multiplier;
	bCanSprint = !bDisableSprint;
}

void ATestCharacter::ShowSnailIcon()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	UImage* SnailImage = Cast<UImage>(OverheadWidgetInstance->GetWidgetFromName(TEXT("Snail")));
	if (SnailImage)
	{
		SnailImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void ATestCharacter::HideSnailIcon()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	UImage* SnailImage = Cast<UImage>(OverheadWidgetInstance->GetWidgetFromName(TEXT("Snail")));
	if (SnailImage)
	{
		SnailImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ATestCharacter::ShowArrowIcon()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	UImage* ArrowImage = Cast<UImage>(OverheadWidgetInstance->GetWidgetFromName(TEXT("Arrow")));
	if (ArrowImage)
	{
		ArrowImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void ATestCharacter::HideArrowIcon()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	UImage* ArrowImage = Cast<UImage>(OverheadWidgetInstance->GetWidgetFromName(TEXT("Arrow")));
	if (ArrowImage)
	{
		ArrowImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ATestCharacter::RemoveDebuff()
{
	SetReverseControl(false);
	SetSpeedMultiplier(1.0f, false);
}
