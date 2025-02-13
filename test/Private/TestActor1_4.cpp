#include "TestActor1_4.h"
#include "TimerManager.h"

ATestActor1_4::ATestActor1_4()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	BodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	BodyMeshComp->SetupAttachment(SceneRoot);

	StaticMeshComp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp1"));
	StaticMeshComp1->SetupAttachment(BodyMeshComp);

	StaticMeshComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp2"));
	StaticMeshComp2->SetupAttachment(StaticMeshComp1);

	TargetRotation1 = FMath::RandRange(-70.0f, 70.0f);
}

void ATestActor1_4::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(RotationTimerHandle, this, &ATestActor1_4::RotateComponents, 0.02f, true);
}

void ATestActor1_4::RotateComponents()
{

	FRotator CurrentRotation1 = StaticMeshComp1->GetRelativeRotation();

	float NextYaw1 = FMath::FInterpTo(CurrentRotation1.Yaw, TargetRotation1, 0.02f, 50.0f);

	StaticMeshComp1->SetRelativeRotation(FRotator(CurrentRotation1.Pitch, NextYaw1, CurrentRotation1.Roll));

	if (FMath::IsNearlyEqual(NextYaw1, TargetRotation1, 1.0f))
	{
		TargetRotation1 = FMath::RandRange(-70.0f, 70.0f);
	}
}
