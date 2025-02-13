#include "ImpedimentBase.h"
#include "Components/SphereComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "ObjectPoolManager.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AImpedimentBase::AImpedimentBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(SceneRoot);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AImpedimentBase::OnOverlapBegin);
	CollisionComponent->SetHiddenInGame(false);
	CollisionComponent->bHiddenInGame = false;
	CollisionComponent->bVisualizeComponent = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneRoot);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AImpedimentBase::BeginPlay()
{
	Super::BeginPlay();

	PlayActionSound();

	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AImpedimentBase::DestroyImpediment, Lifetime, false);
}

void AImpedimentBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter)
	{
		UGameplayStatics::ApplyDamage(PlayerCharacter, DamageAmount, nullptr, this, nullptr);

		PlayOverlapSound(PlayerCharacter);

		DestroyImpediment();
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

void AImpedimentBase::PlayOverlapSound(AActor* TargetActor)
{
	if (OverlapSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, OverlapSound, TargetActor->GetActorLocation());
	}
}

void AImpedimentBase::PlayActionSound()
{
	if (SpawnSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, SpawnSound, GetActorLocation(), FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, SoundAttenuation);
	}
}
