#include "DronController.h"
#include "EnhancedInputSubsystems.h"

ADronController::ADronController()
	:InputMappingContext(nullptr),
	DronMoveAction(nullptr),
	DronLookAction(nullptr),
	DronRotationAction(nullptr)
{
}

void ADronController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
