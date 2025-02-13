#include "SlowingItem.h"
#include "TestCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ASlowingItem::ASlowingItem()
{
}

void ASlowingItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);

    ATestCharacter* Character = Cast<ATestCharacter>(Activator);
    if (Character)
    {
        Character->SetSpeedMultiplier(0.3f, true);
        Character->ShowSnailIcon();
        DestroyItem();
        FTimerHandle DebuffTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            DebuffTimerHandle,
            [WeakCharacter = TWeakObjectPtr<ATestCharacter>(Character)]()
            {
                if (WeakCharacter.IsValid())
                {
                    WeakCharacter.Get()->SetSpeedMultiplier(1.0f, false);
                    WeakCharacter.Get()->HideSnailIcon();
                }
            },
            DebuffDuration,
            false
        );
    }
}
