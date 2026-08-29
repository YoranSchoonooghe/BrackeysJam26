#include "MenuFlowSubsystem.h"

#include "MenuStateBase.h"
#include "Kismet/GameplayStatics.h"

void UMenuFlowSubsystem::SetRootState(TSubclassOf<UMenuStateBase> StateClass)
{
	if (!StateClass)
	{
		return;
	}

	if (UMenuStateBase* Current = GetCurrentState())
	{
		Current->ExitState(this);
	}

	StateStack.Reset();
	StateStack.Add(NewObject<UMenuStateBase>(this, StateClass));

	GetCurrentState()->EnterState(this);
	BroadcastCurrentState();
}

void UMenuFlowSubsystem::PushState(TSubclassOf<UMenuStateBase> StateClass)
{
	if (!StateClass)
	{
		return;
	}

	if (UMenuStateBase* Current = GetCurrentState())
	{
		Current->ExitState(this);
	}

	StateStack.Add(NewObject<UMenuStateBase>(this, StateClass));

	GetCurrentState()->EnterState(this);
	BroadcastCurrentState();
}

void UMenuFlowSubsystem::PopState()
{
	if (StateStack.Num() <= 1)
	{
		return;
	}

	StateStack.Last()->ExitState(this);
	StateStack.Pop();

	GetCurrentState()->EnterState(this);
	BroadcastCurrentState();
}

void UMenuFlowSubsystem::RequestBack()
{
	if (StateStack.Num() > 1)
	{
		PopState();
		return;
	}

	if (UMenuStateBase* Current = GetCurrentState())
	{
		Current->OnBackPressedAtRoot(this);
	}
}

UMenuStateBase* UMenuFlowSubsystem::GetCurrentState() const
{
	return StateStack.Num() > 0 ? StateStack.Last() : nullptr;
}

void UMenuFlowSubsystem::BroadcastCurrentState()
{
	if (UMenuStateBase* Current = GetCurrentState())
	{
		OnMenuStateChanged.Broadcast(Current->GetClass());
	}
}

APlayerController* UMenuFlowSubsystem::GetFirstPlayerController() const
{
	return UGameplayStatics::GetPlayerController(GetGameInstance()->GetWorld(), 0);
}
