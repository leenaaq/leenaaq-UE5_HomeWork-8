#include "TestActor1_5.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ATestActor1_5::ATestActor1_5()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(SceneRoot);
	SpawnPoint->SetRelativeLocation(FVector(0, 0, 50));

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultSphereMesh(TEXT("/Engine/BasicShapes/Sphere"));
	if (DefaultSphereMesh.Succeeded())
	{
		SphereMesh = DefaultSphereMesh.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	if (DefaultMaterial.Succeeded())
	{
		SphereMaterial = DefaultMaterial.Object;
	}
}

void ATestActor1_5::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < PoolSize; i++)
	{
		UStaticMeshComponent* NewSphere = NewObject<UStaticMeshComponent>(this);
		if (NewSphere)
		{
			NewSphere->SetupAttachment(SceneRoot);
			NewSphere->SetStaticMesh(SphereMesh);
			NewSphere->SetMaterial(0, SphereMaterial);
			NewSphere->SetWorldScale3D(FVector(SphereSize * 0.01f));
			NewSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			NewSphere->SetSimulatePhysics(true);
			NewSphere->SetEnableGravity(true);
			NewSphere->SetHiddenInGame(true);
			NewSphere->RegisterComponent();
			SpherePool.Add(NewSphere);
		}
	}

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ATestActor1_5::SpawnSphere, SpawnInterval, true);
}

void ATestActor1_5::SpawnSphere()
{
	for (UStaticMeshComponent* Sphere : SpherePool)
	{
		if (Sphere && Sphere->bHiddenInGame)
		{
			FVector SpawnLocation = SpawnPoint->GetComponentLocation();
			Sphere->SetWorldLocation(SpawnLocation);
			FVector AdjustedVelocity = -GetActorForwardVector() * 1000.0f + FVector(0.0f, 0.0f, 200.0f);

			Sphere->SetHiddenInGame(false);
			Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Sphere->SetSimulatePhysics(true);
			Sphere->SetEnableGravity(true);
			Sphere->SetMassOverrideInKg(NAME_None, Mass);
			Sphere->SetLinearDamping(LinearDamping);
			Sphere->SetAngularDamping(AngularDamping);
			Sphere->SetPhysicsLinearVelocity(AdjustedVelocity);

			FTimerHandle ReturnTimerHandle;
			GetWorldTimerManager().SetTimer(ReturnTimerHandle, FTimerDelegate::CreateUObject(this, &ATestActor1_5::ReturnSphere, Sphere), 1.5f, false);
			return;
		}
	}
}

void ATestActor1_5::ReturnSphere(UStaticMeshComponent* Sphere)
{
	if (Sphere)
	{
		Sphere->SetSimulatePhysics(false);
		Sphere->SetEnableGravity(false);
		Sphere->SetHiddenInGame(true);
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Sphere->SetPhysicsLinearVelocity(FVector::ZeroVector);
		Sphere->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	}
}