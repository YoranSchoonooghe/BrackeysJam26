#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BusQueueComponent.generated.h"

class ANPCCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRACKEYSJAM26_API UBusQueueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBusQueueComponent();

protected:
	virtual void BeginPlay() override;

public:	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void StartNextPassenger();

private:
	TQueue<ANPCCharacter*> PassengersQueue;
		
};
