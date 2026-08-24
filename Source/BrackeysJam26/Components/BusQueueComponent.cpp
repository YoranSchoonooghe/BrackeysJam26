#include "BusQueueComponent.h"
#include <Kismet/GameplayStatics.h>
#include "BrackeysJam26/NPC/NPCCharacter.h"

UBusQueueComponent::UBusQueueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UBusQueueComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundNPCs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPCCharacter::StaticClass(), FoundNPCs);

	for (auto FoundNPC : FoundNPCs)
	{
		auto passenger = Cast<ANPCCharacter>(FoundNPC);
		PassengersQueue.Enqueue(passenger);
	}

	StartNextPassenger();
}

void UBusQueueComponent::StartNextPassenger()
{
	if (PassengersQueue.IsEmpty()) return;

	auto passenger = *PassengersQueue.Peek();
	
	if (passenger)
	{
		passenger->ChangeState(ENPCState::EnterBus);
	}

	PassengersQueue.Pop();
}


//void UBusQueueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//}

