#include "DebuffBaseItem.h"

ADebuffBaseItem::ADebuffBaseItem()
{
	DebuffDuration = 2.0f;
}

void ADebuffBaseItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (IsValid(Activator))
	{
		ApplyDebuff(Activator);

		FTimerHandle DebuffTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			DebuffTimerHandle,
			[this, Activator]()
			{
				RemoveDebuff(Activator);
			},
			DebuffDuration,
			false
		);
	}
	DestroyItem();
}
