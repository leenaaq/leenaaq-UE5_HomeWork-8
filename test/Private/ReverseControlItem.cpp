#include "ReverseControlItem.h"
#include "TestCharacter.h"

AReverseControlItem::AReverseControlItem()
{
}

void AReverseControlItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);

    ATestCharacter* Character = Cast<ATestCharacter>(Activator);
    if (Character)
    {
        Character->SetReverseControl(true);
        Character->ShowArrowIcon();
        DestroyItem();
        FTimerHandle DebuffTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            DebuffTimerHandle,
            [WeakCharacter = TWeakObjectPtr<ATestCharacter>(Character)]()
            {
                if (WeakCharacter.IsValid())
                {
                    WeakCharacter.Get()->SetReverseControl(false);
                    WeakCharacter.Get()->HideArrowIcon();
                }
            },
            DebuffDuration,
            false
        );
    }
}
