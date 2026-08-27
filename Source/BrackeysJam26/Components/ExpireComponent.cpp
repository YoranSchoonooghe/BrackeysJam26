#include "ExpireComponent.h"

UExpireComponent::UExpireComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UExpireComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UExpireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (State)
	{
	case ExpirationState::Disabled:
		break;
	case ExpirationState::Idle:
		UpdateIdleState(DeltaTime);
		break;
	case ExpirationState::Blinking:
		UpdateBlinkingState(DeltaTime);
		break;
	}
}

void UExpireComponent::StartExpire()
{
	State = ExpirationState::Idle;
}

void UExpireComponent::UpdateIdleState(float DeltaTime)
{
	ElapsedTime += DeltaTime;

	if (ElapsedTime >= TimeBeforeBlink)
	{
		State = ExpirationState::Blinking;
	}
}

void UExpireComponent::UpdateBlinkingState(float DeltaTime)
{
	ElapsedTime += DeltaTime;

	if (ElapsedTime >= TimeBeforeExpire)
	{
		if (bDestroyOnExpired)
		{
			GetOwner()->Destroy();
		}
		else
		{
			GetOwner()->SetActorHiddenInGame(true);
		}

		return;
	}

	ElapsedBlinkTime += DeltaTime;

	if (ElapsedBlinkTime >= BlinkFrequency)
	{
		ToggleVisibility();
		ElapsedBlinkTime = 0.0f;
	}
}

void UExpireComponent::ToggleVisibility()
{
	GetOwner()->SetActorHiddenInGame(!GetOwner()->IsHidden());
}

