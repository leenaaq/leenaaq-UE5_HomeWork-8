#include "TestGameMode.h"
#include "TestCharacter.h"
#include "TestPlayerController.h"
#include "MyGameState.h"
//#include "GameFramework/WorldSettings.h"

ATestGameMode::ATestGameMode()
{
    DefaultPawnClass = ATestCharacter::StaticClass(); 
    PlayerControllerClass = ATestPlayerController::StaticClass();
    GameStateClass = AMyGameState::StaticClass();
}

void ATestGameMode::BeginPlay()
{
    Super::BeginPlay();
}