#include "BTT_RequestPermission.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "../../UI/MonitorActor.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "BrackeysJam26/Character/PlayerCharacter.h"
#include "BrackeysJam26/Bus/Bus.h"

EBTNodeResult::Type UBTT_RequestPermission::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComponent) return EBTNodeResult::Failed;

    auto* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APawn* Pawn = AIController->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    auto* NPCCharacter = Cast<ANPCCharacter>(Pawn);
    if (!NPCCharacter) return EBTNodeResult::Failed;

    auto* PlayerChar = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!PlayerChar) return EBTNodeResult::Failed;

    auto* monitorActor = Cast<AMonitorActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AMonitorActor::StaticClass()));
    if (monitorActor)
    {
        monitorActor->ShowScreen(EMonitorScreen::RequestPermission);
    }

    auto* Bus = Cast<ABus>(UGameplayStatics::GetActorOfClass(GetWorld(), ABus::StaticClass()));
    if (Bus)
    {
        Bus->SetPassengerDocsVisibility(true);
    }

    NPCCharacter->ChangeState(ENPCState::Wait);
    PlayerChar->SetTargetNPC(NPCCharacter);

    return EBTNodeResult::Succeeded;
}
