#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"
#include "BTD_IsStateEqualTo.generated.h"

UCLASS()
class BRACKEYSJAM26_API UBTD_IsStateEqualTo : public UBTDecorator
{
	GENERATED_BODY()
	
private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "State")
	ENPCState TargetState{ ENPCState::EnterBus };
};
