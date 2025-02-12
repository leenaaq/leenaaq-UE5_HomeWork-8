#include "TestActor6.h"

ATestActor6::ATestActor6()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	RandomLocationTime = 2.0f;
	
}

void ATestActor6::BeginPlay()
{
	Super::BeginPlay();
	CurrentLocation = GetActorLocation();
	MinRange = CurrentLocation;
	MaxRange = FVector(CurrentLocation.X + 50.0f, CurrentLocation.Y + 50.0f, CurrentLocation.Z + 50.0f);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATestActor6::RandomLocation, RandomLocationTime, true);
}

void ATestActor6::RandomLocation()
{
	FVector RandomL;
	RandomL.X = FMath::RandRange(MinRange.X, MaxRange.X);
	RandomL.Y = FMath::RandRange(MinRange.Y, MaxRange.Y);
	RandomL.Z = FMath::RandRange(MinRange.Z, MaxRange.Z);
	SetActorLocation(RandomL);
}

