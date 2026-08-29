#include "MenuStateBase.h"

#include "MenuFlowSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "../Character/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"

void UMenuStateBase::EnterState(UMenuFlowSubsystem* Flow)
{
	APlayerController* PC = Flow ? Flow->GetFirstPlayerController() : nullptr;
	if (!PC)
	{
		return;
	}

	if (WidgetClass)
	{
		ActiveWidget = CreateWidget<UUserWidget>(PC, WidgetClass);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
				FString::Printf(TEXT("EnterState: PC=%s Widget=%s"),
					PC ? TEXT("valid") : TEXT("NULL"),
					ActiveWidget ? TEXT("valid") : TEXT("NULL")));
		}

		if (ActiveWidget)
		{
			ActiveWidget->AddToViewport();
		}
	}

	PC->SetPause(bPausesGame);
	PC->SetShowMouseCursor(bShowsMouseCursor);

	APlayerCharacter* Character = Cast<APlayerCharacter>(PC->GetPawn());

	if (Character)
	{
		Character->ToggleHUDVisibility(!bShowsMouseCursor);
	}

	if (bShowsMouseCursor)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(ActiveWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void UMenuStateBase::ExitState(UMenuFlowSubsystem* Flow)
{
	if (ActiveWidget)
	{
		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;
	}
}
