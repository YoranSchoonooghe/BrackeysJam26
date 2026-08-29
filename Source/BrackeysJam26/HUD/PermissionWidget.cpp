#include "PermissionWidget.h"
#include <Kismet/GameplayStatics.h>
#include "BrackeysJam26/Character/PlayerCharacter.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"
#include "BrackeysJam26/Bus/BusRouteManager.h"
#include "Components/Button.h"

void UPermissionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	auto* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	auto* NPC = player ? player->GetTargetNPC() : nullptr;

	const bool bWaitingForApproval = NPC && NPC->GetState() == ENPCState::Wait;

	if (AllowButton)
	{
		AllowButton->SetIsEnabled(bWaitingForApproval);
	}

	if (DenyButton)
	{
		DenyButton->SetIsEnabled(bWaitingForApproval);
	}
}

void UPermissionWidget::AllowNPCOnBus()
{
	auto* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player) return;

	auto* NPC = player->GetTargetNPC();
	if (!NPC) return;
	NPC->ChangeState(ENPCState::WalkToSeat);

	OnPassengerAllowed.Broadcast();
}

void UPermissionWidget::DenyNPCFromBus()
{
	auto* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player) return;

	auto* NPC = player->GetTargetNPC();
	if (!NPC) return;

	if (!NPC->PassengerRecord.bIsImposter)
	{
		if (auto* RouteManager = Cast<ABusRouteManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABusRouteManager::StaticClass())))
		{
			RouteManager->RealNPCsDenied++;
		}
	}


	NPC->ChangeState(ENPCState::ExitBus);

	OnPassengerDenied.Broadcast();
}
