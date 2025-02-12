#include "CoinItem.h"
#include "Engine/World.h"
#include "MyGameState.h"
#include "TimerManager.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
	RotationSpeed = 1.0f;
}


void ACoinItem::BeginPlay()
{
	Super::BeginPlay();

	// 일정한 간격으로 RotateItem() 호출 (프레임마다 실행)
	GetWorldTimerManager().SetTimer(RotationTimerHandle, this, &ACoinItem::RotateItem, 0.02f, true);
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (AMyGameState* GameState = World->GetGameState<AMyGameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}
		DestroyItem();
	}
}

void ACoinItem::RotateItem()
{
	if (!StaticMesh) return;

	FRotator NewRotation = FRotator(0.0f, RotationSpeed * 0.02f, 0.0f);

	StaticMesh->AddLocalRotation(NewRotation);
}
