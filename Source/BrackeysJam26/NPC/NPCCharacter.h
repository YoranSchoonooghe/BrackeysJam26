#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../DataManager.h"
#include "../InteractableInterface.h"
#include "NPCCharacter.generated.h"

class ABusSeat;
class UMaterialInterface;

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
class BRACKEYSJAM26_API ANPCCharacter : public ACharacter, public IInteractableInterface
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

	UFUNCTION(BlueprintCallable, Category = "NPC")
	bool IsSitting() const { return NPCState == ENPCState::Sitting; }

	UFUNCTION()
	bool Eject(float Force);

	UFUNCTION()
	void SetCurrentSeat(ABusSeat* Seat) { CurrentSeat = Seat; }

	UFUNCTION()
	void Despawn();

	virtual void Interact_Implementation() override;

	UFUNCTION()
	void SetHighlighted(bool bHighlighted);

	UPROPERTY(EditAnywhere, Category = "Highlight")
	TObjectPtr<UMaterialInterface> OutlineMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FPassengerRecord PassengerRecord;

private:
	void AssignSeat();

	ENPCState NPCState{ ENPCState::Wait };

	UPROPERTY()
	TObjectPtr<ABusSeat> CurrentSeat;
};
