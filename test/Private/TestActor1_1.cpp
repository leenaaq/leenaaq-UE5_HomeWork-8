#include "TestActor1_1.h"
#include "TimerManager.h"

ATestActor1_1::ATestActor1_1()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	BodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	BodyMeshComp->SetupAttachment(SceneRoot);

	StaticMeshComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh2"));
	StaticMeshComp2->SetupAttachment(BodyMeshComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(StaticMeshComp2);

	MoveSpeed = 5.0f;
	MaxDistance = 50.0f;
	bMovingRight = true;
}

void ATestActor1_1::BeginPlay()
{
	Super::BeginPlay();
	InitialX = StaticMeshComp2->GetRelativeLocation().X;

	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &ATestActor1_1::MovePlatform, 0.02f, true);
}

void ATestActor1_1::MovePlatform()
{
	FVector CurrentLocation = StaticMeshComp2->GetRelativeLocation();
	float TargetX = bMovingRight ? (InitialX + MaxDistance) : (InitialX - MaxDistance);

	float NextX = FMath::FInterpTo(CurrentLocation.X, TargetX, 0.02f, 3.0f);

	StaticMeshComp2->SetRelativeLocation(FVector(NextX, CurrentLocation.Y, CurrentLocation.Z));

	if (FMath::IsNearlyEqual(NextX, TargetX, 1.0f))
	{
		bMovingRight = !bMovingRight;
	}
}
