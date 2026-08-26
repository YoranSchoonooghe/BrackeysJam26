#include "BTT_Despawn.h"
#include "AIController.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"

EBTNodeResult::Type UBTT_Despawn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    auto* NPC = Cast<ANPCCharacter>(AIController->GetCharacter());
    if (!NPC) return EBTNodeResult::Failed;
    
    NPC->Despawn();

    return EBTNodeResult::Succeeded;
}
