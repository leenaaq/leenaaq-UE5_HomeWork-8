#include "TestActor1.h"
#include "TimerManager.h"

ATestActor1::ATestActor1()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	BodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	BodyMeshComp->SetupAttachment(SceneRoot);

	StaticMeshComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh2"));
	StaticMeshComp2->SetupAttachment(BodyMeshComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(StaticMeshComp2);

	MoveSpeed = 10.0f;
	MaxHeight = 200.0f;
	bMovingUp = true;
}

void ATestActor1::BeginPlay()
{
	Super::BeginPlay();

	InitialZ = StaticMeshComp2->GetRelativeLocation().Z;

	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &ATestActor1::MovePlatform, 0.05f, true);
}

void ATestActor1::MovePlatform()
{
	FVector CurrentLocation = StaticMeshComp2->GetRelativeLocation();
	float NextHeight = CurrentLocation.Z + (bMovingUp ? MoveSpeed : -MoveSpeed);

	if (NextHeight >= InitialZ + MaxHeight)
	{
		bMovingUp = false;
	}
	else if (NextHeight <= InitialZ)
	{
		bMovingUp = true;
	}

	StaticMeshComp2->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, NextHeight));
}
