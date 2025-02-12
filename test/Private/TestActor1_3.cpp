#include "TestActor1_3.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ATestActor1_3::ATestActor1_3()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(SceneRoot);

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
    BaseMesh->SetupAttachment(SceneRoot);

    DetailMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DetailMesh"));
    DetailMesh->SetupAttachment(SceneRoot);

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATestActor1_3::OnOverlapBegin);

    static ConstructorHelpers::FObjectFinder<USoundBase> Sound(TEXT("/Game/Sounds/WoodBoom.WoodBoom"));
    if (Sound.Succeeded())
    {
        WoodBoomSound = Sound.Object;
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh1(TEXT("/Game/Meshes/BaseMesh.BaseMesh"));
    if (Mesh1.Succeeded())
    {
        BaseMesh->SetStaticMesh(Mesh1.Object);
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh2(TEXT("/Game/Meshes/DetailMesh.DetailMesh"));
    if (Mesh2.Succeeded())
    {
        DetailMesh->SetStaticMesh(Mesh2.Object);
    }
}

void ATestActor1_3::BeginPlay()
{
    Super::BeginPlay();
}

void ATestActor1_3::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
    {
        GetWorld()->GetTimerManager().SetTimer(
            DestroyTimerHandle, this, &ATestActor1_3::PlaySoundAndDestroy, 1.0f, false);
    }
}

void ATestActor1_3::PlaySoundAndDestroy()
{
    if (WoodBoomSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, WoodBoomSound, GetActorLocation());
    }

    if (DestroyEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            DestroyEffect,
            GetActorLocation(),
            GetActorRotation(),
            FVector(10.0f, 10.0f, 10.0f),
            true
        );
    }

    GetWorld()->GetTimerManager().SetTimer(
        DestroyTimerHandle, this, &ATestActor1_3::DestroyActor, 0.3f, false);
}

void ATestActor1_3::DestroyActor()
{
    Destroy();
}
