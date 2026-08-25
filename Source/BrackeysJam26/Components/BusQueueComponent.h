#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BusQueueComponent.generated.h"

class ANPCCharacter;
class ABusStop;

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

	UFUNCTION(BlueprintCallable, Category = "Passengers")
	void SpawnPassengersForStop(ABusStop* Stop = nullptr);

	UPROPERTY(EditAnywhere, Category = "Passengers")
	TSubclassOf<ANPCCharacter> PassengerClass;

	UPROPERTY(EditAnywhere, Category = "Passengers")
	int32 MinPassengers = 1;

	UPROPERTY(EditAnywhere, Category = "Passengers")
	int32 MaxPassengers = 4;

	UPROPERTY(EditAnywhere, Category = "Passengers")
	TObjectPtr<ABusStop> CurrentBusStop;

	UPROPERTY(EditAnywhere, Category = "Passengers")
	float SpawnRadius = 200.0f;

private:
	TQueue<ANPCCharacter*> PassengersQueue;

};
