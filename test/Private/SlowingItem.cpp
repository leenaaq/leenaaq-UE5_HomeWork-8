#include "SlowingItem.h"
#include "TestCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ASlowingItem::ASlowingItem()
{
	DebuffDuration = 2.5f; // 지속시간
}

void ASlowingItem::ApplyDebuff(AActor* AffectedActor)
{
	ATestCharacter* Character = Cast<ATestCharacter>(AffectedActor);
	if (Character)
	{
		Character->SetSpeedMultiplier(0.3f, true);
		Character->ShowSnailIcon();
	}
}

void ASlowingItem::RemoveDebuff(AActor* AffectedActor)
{
	ATestCharacter* Character = Cast<ATestCharacter>(AffectedActor);
	if (Character)
	{
		Character->SetSpeedMultiplier(1.0f, false);
		Character->HideSnailIcon();
	}
}
