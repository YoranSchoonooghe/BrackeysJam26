#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacter.generated.h"

UENUM(BlueprintType)
enum class ENPCState : uint8
{
	Wait,
	EnterBus,
	WalkToSeat,
	Sitting,
	ExitBus
};

UCLASS()
class BRACKEYSJAM26_API ANPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANPCCharacter();

protected:
	virtual void BeginPlay() override;

public:
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ChangeState(ENPCState NewState);

	UFUNCTION()
	ENPCState GetState() const { return NPCState; }

	UFUNCTION()
	void Eject(float Force);

private:
	ENPCState NPCState{ ENPCState::EnterBus };
};
