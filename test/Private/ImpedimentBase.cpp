#include "ImpedimentBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AImpedimentBase::AImpedimentBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(SceneRoot);
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AImpedimentBase::OnOverlapBegin);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneRoot);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DamageAmount = 10.0f;
	Lifetime = 5.0f;
}

void AImpedimentBase::BeginPlay()
{
	Super::BeginPlay();

	FVector CurrentLocation = GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("AImpedimentBase BeginPlay Location: %s"), *CurrentLocation.ToString());

	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AImpedimentBase::DestroyImpediment, Lifetime, false);
}


void AImpedimentBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter)
	{
		UGameplayStatics::ApplyDamage(PlayerCharacter, DamageAmount, nullptr, this, nullptr);
	}
}

void AImpedimentBase::DestroyImpediment()
{
	AObjectPoolManager* PoolManager = Cast<AObjectPoolManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AObjectPoolManager::StaticClass())
	);

	if (PoolManager)
	{
		PoolManager->ReturnPooledObject(this);
	}
	else
	{
		Destroy();
	}
}
