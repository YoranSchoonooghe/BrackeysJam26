#include "BTT_GiveDocs.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTT_GiveDocs::UBTT_GiveDocs()
{
    bNotifyTick = false;

    bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_GiveDocs::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (!GiveDocsAnimMontage) return EBTNodeResult::Failed;

    auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComponent) return EBTNodeResult::Failed;

    auto* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APawn* Pawn = AIController->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

	auto* Character = Cast<ACharacter>(Pawn);
    if (!Character) return EBTNodeResult::Failed;


	USkeletalMeshComponent* Mesh = Character->GetMesh();
	if (!Mesh) return EBTNodeResult::Failed;

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (!AnimInstance) return EBTNodeResult::Failed;

	const float MontageLength = AnimInstance->Montage_Play(
		GiveDocsAnimMontage,
		PlayRate
	);


	FOnMontageEnded EndDelegate;

	EndDelegate.BindLambda(
		[this, &OwnerComp](UAnimMontage* Montage, bool bInterrupted)
		{
			OnMontageEnded(
				Montage,
				bInterrupted,
				&OwnerComp
			);
		}
	);

	AnimInstance->Montage_SetEndDelegate(
		EndDelegate,
		GiveDocsAnimMontage
	);

	return EBTNodeResult::InProgress;
}

void UBTT_GiveDocs::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp)
{
	if (!OwnerComp) return;

	if (bInterrupted)
	{
		FinishLatentTask(
			*OwnerComp,
			EBTNodeResult::Aborted
		);
	}
	else
	{
		FinishLatentTask(
			*OwnerComp,
			EBTNodeResult::Succeeded
		);
	}
}
