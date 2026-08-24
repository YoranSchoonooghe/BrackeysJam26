#include "PermissionWidget.h"
#include <Kismet/GameplayStatics.h>
#include "BrackeysJam26/Character/PlayerCharacter.h"
#include "BrackeysJam26/Character/DefaultPlayerController.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"

void UPermissionWidget::AllowNPCOnBus()
{
	auto* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player) return;

	auto* NPC = player->GetTargetNPC();
	if (!NPC) return;
	NPC->ChangeState(ENPCState::WalkToSeat);

	//auto* playerController = Cast<ADefaultPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//playerController->HidePermissionWidget();

	auto* monitorActor = Cast<AMonitorActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AMonitorActor::StaticClass()));
	if (monitorActor)
	{
		monitorActor->ShowScreen(EMonitorScreen::Empty);
	}
}

void UPermissionWidget::DenyNPCFromBus()
{
	auto* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player) return;

	auto* NPC = player->GetTargetNPC();
	if (!NPC) return;
	NPC->ChangeState(ENPCState::ExitBus);

	//auto* playerController = Cast<ADefaultPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//playerController->HidePermissionWidget();


	auto* monitorActor = Cast<AMonitorActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AMonitorActor::StaticClass()));
	if (monitorActor)
	{
		monitorActor->ShowScreen(EMonitorScreen::Empty);
	}
}
