#include "DefaultPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "BrackeysJam26/HUD/PermissionWidget.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"

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

		if (LookAction)
			EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADefaultPlayerController::Look);

		if (InteractAction)
			EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &ADefaultPlayerController::Interact);
	}
}

void ADefaultPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CachedPlayerCharacter = Cast<APlayerCharacter>(InPawn);

	if (PermissionWidgetClass)
	{
		PermissionWidgetInstance = CreateWidget<UPermissionWidget>(this, PermissionWidgetClass);
	}
}

void ADefaultPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	CachedPlayerCharacter = nullptr;
}

void ADefaultPlayerController::ShowPermissionWidget(ANPCCharacter* NPC)
{
	if (!PermissionWidgetInstance) return;
	
	CachedPlayerCharacter->SetTargetNPC(NPC);
	PermissionWidgetInstance->AddToViewport();
}

void ADefaultPlayerController::HidePermissionWidget()
{
	if (!PermissionWidgetInstance) return;

	PermissionWidgetInstance->RemoveFromParent();
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

void ADefaultPlayerController::Look(const FInputActionValue& Value)
{
	if (!CachedPlayerCharacter) return;

	FVector2D CameraMovementVector = Value.Get<FVector2D>();
	if (CameraMovementVector.Length() < 0.1f) return;

	CachedPlayerCharacter->Look(CameraMovementVector);
}

void ADefaultPlayerController::Interact()
{
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->Interact();
	}
}