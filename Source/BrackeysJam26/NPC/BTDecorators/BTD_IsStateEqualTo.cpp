#include "BTD_IsStateEqualTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

bool UBTD_IsStateEqualTo::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComponent) return false;

    auto* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return false;

    APawn* Pawn = AIController->GetPawn();
    if (!Pawn) return false;

    auto* NPCCharacter = Cast<ANPCCharacter>(Pawn);
    if (!NPCCharacter) return false;

    if (NPCCharacter->GetState() == TargetState)
        return true;

	return false;
}
