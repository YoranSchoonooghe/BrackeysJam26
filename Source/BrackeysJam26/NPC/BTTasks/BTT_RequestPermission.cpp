#include "BTT_RequestPermission.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "BrackeysJam26/Character/DefaultPlayerController.h"

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

    auto* playerController = Cast<ADefaultPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!playerController) return EBTNodeResult::Failed;

    NPCCharacter->ChangeState(ENPCState::Wait);
    
    playerController->ShowPermissionWidget(NPCCharacter);

    return EBTNodeResult::Succeeded;
}
