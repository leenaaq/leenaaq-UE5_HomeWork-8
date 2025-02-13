#include "TestActor1_2.h"
#include "TimerManager.h"

ATestActor1_2::ATestActor1_2()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	BodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	BodyMeshComp->SetupAttachment(SceneRoot);

	StaticMeshComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh2"));
	StaticMeshComp2->SetupAttachment(BodyMeshComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(StaticMeshComp2);

	RotationSpeed = 45.0f;
}

void ATestActor1_2::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(RotationTimerHandle, this, &ATestActor1_2::RotatePlatform, 0.02f, true);
}

void ATestActor1_2::RotatePlatform()
{

	FRotator CurrentRotation = StaticMeshComp->GetRelativeRotation();
	float DeltaRotation = RotationSpeed * 0.02f;
	FRotator NewRotation = CurrentRotation + FRotator(0.0f, DeltaRotation, 0.0f);
	StaticMeshComp->SetRelativeRotation(NewRotation);
}
