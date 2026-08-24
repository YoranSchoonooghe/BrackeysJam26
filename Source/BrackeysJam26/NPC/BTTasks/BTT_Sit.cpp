#include "BTT_Sit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"
#include "BrackeysJam26/Bus/BusSeat.h"

EBTNodeResult::Type UBTT_Sit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComponent) return EBTNodeResult::Failed;

    auto* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APawn* Pawn = AIController->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    auto* NPCCharacter = Cast<ANPCCharacter>(Pawn);
    if (!NPCCharacter) return EBTNodeResult::Failed;

    auto* Seat = Cast<ABusSeat>(BlackboardComponent->GetValueAsObject(TEXT("TargetSeat")));
    if (!Seat) return EBTNodeResult::Failed;

    NPCCharacter->ChangeState(ENPCState::Sitting);
    Seat->Occupy(NPCCharacter);

	return EBTNodeResult::Succeeded;
}
