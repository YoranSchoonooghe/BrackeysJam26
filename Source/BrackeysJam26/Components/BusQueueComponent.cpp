#include "BusQueueComponent.h"
#include <Kismet/GameplayStatics.h>
#include "BrackeysJam26/NPC/NPCCharacter.h"
#include "BrackeysJam26/UI/MonitorActor.h"

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
	auto* monitorActor = Cast<AMonitorActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AMonitorActor::StaticClass()));

	if (PassengersQueue.IsEmpty())
	{
		if (monitorActor)
		{
			monitorActor->ShowScreen(EMonitorScreen::CameraView);
		}

		return;
	}

	auto passenger = *PassengersQueue.Peek();

	if (passenger)
	{
		passenger->ChangeState(ENPCState::EnterBus);
	}

	PassengersQueue.Pop();

	if (monitorActor)
	{
		monitorActor->ShowScreen(EMonitorScreen::Empty);
	}
}


//void UBusQueueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//}

