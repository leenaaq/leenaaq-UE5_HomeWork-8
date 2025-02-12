#include "ReverseControlItem.h"
#include "TestCharacter.h"

AReverseControlItem::AReverseControlItem()
{
	DebuffDuration = 2.0f;
}

void AReverseControlItem::ApplyDebuff(AActor* AffectedActor)
{
	ATestCharacter* Character = Cast<ATestCharacter>(AffectedActor);
	if (Character)
	{
		Character->SetReverseControl(true);
		Character->ShowArrowIcon();
	}
}

void AReverseControlItem::RemoveDebuff(AActor* AffectedActor)
{
	ATestCharacter* Character = Cast<ATestCharacter>(AffectedActor);
	if (Character)
	{
		Character->SetReverseControl(false);
		Character->HideArrowIcon();
	}
}
