#include "TestGameState.h"
#include "UObject/ConstructorHelpers.h"

ATestGameState::ATestGameState()
{
    static ConstructorHelpers::FObjectFinder<USoundBase> BGM(TEXT("/Game/Sounds/bgm.bgm"));
    if (BGM.Succeeded())
    {
        BackgroundMusic = BGM.Object;
    }
}

void ATestGameState::BeginPlay()
{
    Super::BeginPlay();

    if (BackgroundMusic)
    {
        UGameplayStatics::PlaySound2D(this, BackgroundMusic);
    }
}
