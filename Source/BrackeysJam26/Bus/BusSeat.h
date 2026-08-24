#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BusSeat.generated.h"

class ANPCCharacter;

UCLASS()
class BRACKEYSJAM26_API ABusSeat : public AActor
{
	GENERATED_BODY()
	
public:	
	ABusSeat();

protected:
	virtual void BeginPlay() override;

public:	
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Seat")
	void Occupy(ANPCCharacter* NPCCharacter);
	UFUNCTION(BlueprintCallable, Category = "Seat")
	void Leave();
	UFUNCTION(BlueprintCallable, Category = "Seat")
	bool IsOccupied() const;

private:
	ANPCCharacter* Occupant;

};
