#include "DronPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DronController.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


ADronPawn::ADronPawn()
{
	DronSpeed =	50.0f;

	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = CapsuleComp;
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	bCanMoveDown = true;
	FootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Foot"));
	FootComp->SetupAttachment(CapsuleComp);
	FootComp->SetCollisionObjectType(ECC_Pawn);
	FootComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	FootComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	FootComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);


	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
}

void ADronPawn::BeginPlay()
{
	Super::BeginPlay();
	if (!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("controller"));
	}

	FootComp->OnComponentBeginOverlap.AddDynamic(this, &ADronPawn::OnFootOverlap);
	FootComp->OnComponentEndOverlap.AddDynamic(this, &ADronPawn::OnFootEndOverlap);
}

void ADronPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bOnGround = CheckIfOnGround();

	bCanMoveDown = !bOnGround;

	if (bCanMoveDown)
	{
		FRotator CurrentRotation = GetActorRotation();
		FVector GravityDirection = GetActorUpVector() * -1.0f;

		GravityDirection += GetActorRightVector() * (CurrentRotation.Roll / 30.0f);
		GravityDirection += GetActorForwardVector() * (-CurrentRotation.Pitch / 30.0f);

		GravityDirection.Normalize();

		AddMovementInput(GravityDirection, 10.f * DeltaTime);
	}
}







void ADronPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ADronController* PlayerController = Cast<ADronController>(GetController()))
		{
			if (PlayerController->DronMoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->DronMoveAction,
					ETriggerEvent::Triggered,
					this,
					&ADronPawn::DronMove
				);
			}
			if (PlayerController->DronLookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->DronLookAction,
					ETriggerEvent::Triggered,
					this,
					&ADronPawn::DronLook
				);
			}
			if (PlayerController->DronRotationAction)
			{
				EnhancedInput->BindAction(
					PlayerController->DronRotationAction,
					ETriggerEvent::Triggered,
					this,
					&ADronPawn::DronRotation
				);
			}
		}
	}
}


void ADronPawn::DronMove(const FInputActionValue& Value)
{
	if (!Controller) return;

	FVector2D MoveInput = Value.Get<FVector2D>();
	FRotator CurrentRotation = GetActorRotation();
	FVector MoveDirection = FVector::ZeroVector;

	float PitchAngle = CurrentRotation.Pitch;
	MoveDirection += GetActorForwardVector() * (-PitchAngle / 30.0f);

	float RollAngle = CurrentRotation.Roll;
	MoveDirection += GetActorRightVector() * (RollAngle / 30.0f);

	if (FMath::Abs(MoveInput.X) > KINDA_SMALL_NUMBER)
	{
		MoveDirection += CurrentRotation.Vector() * MoveInput.X;
	}
	if (FMath::Abs(MoveInput.Y) > KINDA_SMALL_NUMBER)
	{
		MoveDirection += FRotationMatrix(CurrentRotation).GetScaledAxis(EAxis::Y) * MoveInput.Y;
	}

	float ZAxisValue = Value.Get<FVector>().Z;

	if (ZAxisValue < 0 && !bCanMoveDown)
	{
		return;
	}
	if (FMath::Abs(ZAxisValue) > KINDA_SMALL_NUMBER)
	{
		MoveDirection += FVector(0, 0, 1) * ZAxisValue;
	}

	if (!MoveDirection.IsZero())
	{
		AddMovementInput(MoveDirection.GetSafeNormal(), DronSpeed * GetWorld()->GetDeltaSeconds());
	}
}





void ADronPawn::DronLook(const FInputActionValue& value)
{
	if (!Controller) return;

	FVector2D LookInput = value.Get<FVector2D>() * 1.0f;
	FRotator NewControlRotation = Controller->GetControlRotation();

	NewControlRotation.Yaw += LookInput.X;
	NewControlRotation.Pitch = FMath::Clamp(NewControlRotation.Pitch + -LookInput.Y, -80.0f, 80.0f);

	Controller->SetControlRotation(NewControlRotation);
	if (SpringArmComp)
	{
		SpringArmComp->SetWorldRotation(NewControlRotation);
	}
}


void ADronPawn::DronRotation(const FInputActionValue& Value)
{
	if (!Controller) return;

	float RotationSpeed = 2.0f;
	FVector LookInput = Value.Get<FVector>();
	FRotator CurrentRotation = GetActorRotation();

	// Pitch (앞뒤 기울기)
	CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch + (-LookInput.Y * RotationSpeed), -30.0f, 30.0f);

	// Roll (좌우 기울기)
	CurrentRotation.Roll = FMath::Clamp(CurrentRotation.Roll + (LookInput.X * RotationSpeed), -30.0f, 30.0f);

	// Yaw (회전)
	CurrentRotation.Yaw += LookInput.Z * RotationSpeed;

	SetActorRotation(CurrentRotation);
}


void ADronPawn::OnFootOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		bCanMoveDown = false;

		FloatingMovement->Velocity = FVector::ZeroVector;
		CapsuleComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

		CapsuleComp->SetSimulatePhysics(false);
		SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));
		CapsuleComp->SetSimulatePhysics(true);

		UE_LOG(LogTemp, Warning, TEXT("Dron Landed - Rotation Reset & Physics Stabilized"));
	}
}




void ADronPawn::OnFootEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		bCanMoveDown = true;

	}
}

bool ADronPawn::CheckIfOnGround()
{
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, CapsuleComp->GetScaledCapsuleHalfHeight() + 5.0f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		Start, End,
		FQuat::Identity,
		ECC_WorldStatic,
		FCollisionShape::MakeCapsule(CapsuleComp->GetScaledCapsuleRadius(), 5.0f),
		Params
	);

	return bHit;
}

