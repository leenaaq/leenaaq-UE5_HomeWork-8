#include "TestActor4.h"

ATestActor4::ATestActor4()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	//FVector CurrentLocation = GetActorLocation();

	PrimaryActorTick.bCanEverTick = true;

	MoveSpeed = 10.0f;
	MaxHeight = 100.0f;
}

void ATestActor4::BeginPlay()
{
	Super::BeginPlay();

	this->CurrentLocation = GetActorLocation();
}

void ATestActor4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(MoveSpeed))
	{
		AddActorLocalOffset(FVector(0.0f, 0.0f, MoveSpeed * DeltaTime));
		CheckAndResetHeight();
	}
}

void ATestActor4::CheckAndResetHeight()
{
	if (GetActorLocation().Z >= MaxHeight || GetActorLocation().Z <= CurrentLocation.Z)
	{
		MoveSpeed *= -1;
	}
}
