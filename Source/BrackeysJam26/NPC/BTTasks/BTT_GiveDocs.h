#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_GiveDocs.generated.h"

UCLASS()
class BRACKEYSJAM26_API UBTT_GiveDocs : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTT_GiveDocs();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> GiveDocsAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float PlayRate{ 1.0f };

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp);

};
