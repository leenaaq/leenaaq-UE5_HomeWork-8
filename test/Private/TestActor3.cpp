#include "TestActor3.h"

ATestActor3::ATestActor3()
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

void ATestActor3::BeginPlay()
{
	Super::BeginPlay();
	this->CurrentLocation = GetActorLocation();
}

void ATestActor3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(MoveSpeed))
	{
		AddActorLocalOffset(FVector(0.0f, 0.0f, MoveSpeed * DeltaTime));
		CheckAndResetHeight();
	}
}

void ATestActor3::CheckAndResetHeight()
{
	if (GetActorLocation().Z >= MaxHeight)
	{
		SetActorLocation(CurrentLocation);
	}
}
