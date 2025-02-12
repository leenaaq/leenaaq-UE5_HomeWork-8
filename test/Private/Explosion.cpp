#include "Explosion.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AExplosion::AExplosion()
{
	DamageAmount = 50.0f; 
	Lifetime = 3.0f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Models/ExplosionMesh.ExplosionMesh"));
	if (MeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(MeshAsset.Object);
	}
}

void AExplosion::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter)
	{
		UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, GetActorLocation(), 300.0f, nullptr, {}, this);
	}
}
