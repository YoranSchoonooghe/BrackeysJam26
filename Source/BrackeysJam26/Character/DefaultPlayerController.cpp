#include "DefaultPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PlayerCharacter.h"

ADefaultPlayerController::ADefaultPlayerController()
{
}

void ADefaultPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void ADefaultPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (MoveAction)
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADefaultPlayerController::Move);

		if (JumpAction)
			EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ADefaultPlayerController::Jump);
	}
}

void ADefaultPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CachedPlayerCharacter = Cast<APlayerCharacter>(InPawn);
}

void ADefaultPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	CachedPlayerCharacter = nullptr;
}

void ADefaultPlayerController::Move(const FInputActionValue& Value)
{
	if (!CachedPlayerCharacter) return;

	FVector2D MovementVector = Value.Get<FVector2D>();
	if (MovementVector.Length() < 0.1f) return;

	CachedPlayerCharacter->Move(MovementVector);
}

void ADefaultPlayerController::Jump()
{
	if (!CachedPlayerCharacter) return;

	CachedPlayerCharacter->Jump();
}
