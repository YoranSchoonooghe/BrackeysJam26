#include "PermissionWidget.h"
#include <Kismet/GameplayStatics.h>
#include "BrackeysJam26/Character/PlayerCharacter.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"

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
	NPC->ChangeState(ENPCState::ExitBus);

	OnPassengerDenied.Broadcast();
}
