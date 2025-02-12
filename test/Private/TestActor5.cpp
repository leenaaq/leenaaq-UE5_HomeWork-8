#include "TestActor5.h"

ATestActor5::ATestActor5()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	DisappearTime = 2.0f;
}

void ATestActor5::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATestActor5::HidePlatform, DisappearTime, false);
}

void ATestActor5::HidePlatform()
{
	SetActorHiddenInGame(true);
}

