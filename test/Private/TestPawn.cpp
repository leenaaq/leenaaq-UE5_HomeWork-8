#include "TestPawn.h"
#include "Camera/CameraComponent.h"
#include "TestPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/FloatingPawnMovement.h"


ATestPawn::ATestPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캡슐
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = CapsuleComp;
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComp->SetCollisionObjectType(ECC_Pawn);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Destructible, ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CapsuleComp->SetEnableGravity(false);



	// 발
	FootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Foot"));
	FootComp->SetupAttachment(CapsuleComp);
	FootComp->SetCollisionResponseToAllChannels(ECR_Block);
	FootComp->SetBoxExtent(FVector(10.0f, 10.0f, 5.0f));
	FootComp->SetRelativeLocation(FVector(0.0f, 0.0f, -CapsuleComp->GetScaledCapsuleHalfHeight()));

	// 스켈레탈 메시
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);

	// 스프링암
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	//SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = false;

	// 카메라
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	// 이동
	NormalSpeed = 400.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CapsuleComp;
	MovementComponent->MaxSpeed = NormalSpeed;

	// 점프
	JumpForce = 3000.0f;
	bIsJumping = false;

	// 시점
	LookSpeed = 1.0f;
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

}

void ATestPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ATestPlayerController* PlayerController = Cast<ATestPlayerController>(GetController()))
		{
			if (Controller && PlayerController && PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ATestPawn::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ATestPawn::StartJump
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ATestPawn::StopJump
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ATestPawn::StartSprint
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ATestPawn::StopSprint
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ATestPawn::Look
				);
			}
		}
	}
}

void ATestPawn::Move(const FInputActionValue& value)
{
	if (!Controller || !MovementComponent) return;

	FVector2D MoveInput = value.Get<FVector2D>();
	FRotator ControlRotation = Controller->GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	if (FMath::Abs(MoveInput.X) > KINDA_SMALL_NUMBER || FMath::Abs(MoveInput.Y) > KINDA_SMALL_NUMBER)
	{
		FVector MoveDirection =
			FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::X) * MoveInput.X +
			FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y) * MoveInput.Y;

		FVector TargetLocation = GetActorLocation() + MoveDirection * MovementComponent->MaxSpeed * GetWorld()->GetDeltaSeconds();

		if (!CheckCapsuleCollision(TargetLocation))
		{
			SetActorLocation(TargetLocation, true);
		}
	}
}



void ATestPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float GravityStrength = 9800.f;

	bool bOnGround = CheckIfOnGround();

	if (!bOnGround)
	{
		AddMovementInput(FVector(0, 0, -1), GravityStrength * DeltaTime);
		bIsJumping = true;
	}
	else
	{
		bIsJumping = false;
	}
}

bool ATestPawn::CheckIfOnGround()
{
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, CapsuleComp->GetScaledCapsuleHalfHeight() + 10.f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	return bHit;
}



void ATestPawn::StartJump(const FInputActionValue& value)
{
	if (!bIsJumping && CheckIfOnGround()) 
	{
		bIsJumping = true;
		MovementComponent->Velocity.Z = JumpForce;
	}
}

void ATestPawn::StopJump(const FInputActionValue& value)
{
	if (FootComp)
	{
		TArray<AActor*> OverlappingActors;
		FootComp->GetOverlappingActors(OverlappingActors);

		if (OverlappingActors.Num() > 0)
		{
			bIsJumping = false;
		}
	}
}






void ATestPawn::Look(const FInputActionValue& value)
{
	if (!Controller) return;

	FVector2D LookInput = value.Get<FVector2D>() * LookSpeed;
	FRotator NewControlRotation = Controller->GetControlRotation();
	NewControlRotation.Yaw += LookInput.X;
	NewControlRotation.Pitch = FMath::Clamp(NewControlRotation.Pitch + -LookInput.Y, -80.0f, 80.0f);

	Controller->SetControlRotation(NewControlRotation);
	if (SpringArmComp)
	{
		SpringArmComp->SetWorldRotation(NewControlRotation);
	}
}


void ATestPawn::StartSprint(const FInputActionValue& value)
{
	if (MovementComponent)
	{
		MovementComponent->MaxSpeed = SprintSpeed;
	}
}

void ATestPawn::StopSprint(const FInputActionValue& value)
{
	if (MovementComponent)
	{
		MovementComponent->MaxSpeed = NormalSpeed;
	}
}

void ATestPawn::BeginPlay()
{
	Super::BeginPlay();

	if (FootComp)
	{
		FootComp->OnComponentBeginOverlap.AddDynamic(this, &ATestPawn::OnFootBeginOverlap);
		FootComp->OnComponentEndOverlap.AddDynamic(this, &ATestPawn::OnFootEndOverlap);

		FootComp->SetHiddenInGame(false);
		FootComp->SetVisibility(true);
	}

	if (!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("controller"));
	}
}

void ATestPawn::OnFootBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && !OtherActor->IsA(ATestPawn::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnFootBeginOverlap %s"), *OtherActor->GetName());
	}
}

void ATestPawn::OnFootEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && !OtherActor->IsA(ATestPawn::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnFootEndOverlap %s"), *OtherActor->GetName());
	}
}


bool ATestPawn::CheckCapsuleCollision(FVector TargetLocation)
{
	FHitResult Hit;

	float CapsuleRadius = CapsuleComp->GetScaledCapsuleRadius();
	float CapsuleHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		GetActorLocation(), TargetLocation,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
		Params
	);
	return bHit;
}