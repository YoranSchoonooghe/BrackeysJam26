#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCAIController.generated.h"

UCLASS()
class BRACKEYSJAM26_API ANPCAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ANPCAIController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BehaviorTree")
	TObjectPtr<UBehaviorTree> BTNPC;

private:
	void InitBBKeys();
};
