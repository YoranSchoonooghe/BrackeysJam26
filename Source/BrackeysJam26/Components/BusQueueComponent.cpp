#include "BusQueueComponent.h"
#include <Kismet/GameplayStatics.h>
#include "BrackeysJam26/NPC/NPCCharacter.h"
#include "BrackeysJam26/UI/MonitorActor.h"
#include "BrackeysJam26/Bus/Bus.h"
#include "BrackeysJam26/Bus/BusStop.h"

UBusQueueComponent::UBusQueueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UBusQueueComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnPassengersForStop();
}

void UBusQueueComponent::SpawnPassengersForStop(ABusStop* Stop)
{
	if (Stop)
	{
		CurrentBusStop = Stop;
	}

	ANPCCharacter* Leftover = nullptr;
	while (PassengersQueue.Dequeue(Leftover))
	{
		if (Leftover)
		{
			Leftover->Destroy();
		}
	}

	if (!PassengerClass) return;

	auto* Bus = Cast<ABus>(GetOwner());

	const int32 PassengerCount = FMath::RandRange(MinPassengers, MaxPassengers);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const FRotator SpawnRotation = Bus ? Bus->GetActorRotation() : FRotator::ZeroRotator;

	for (int32 i = 0; i < PassengerCount; i++)
	{
		FVector SpawnLocation;

		if (CurrentBusStop)
		{
			SpawnLocation = CurrentBusStop->GetRandomSpawnPoint();
		}
		else if (Bus)
		{
			const FVector2D RandomOffset = FMath::RandPointInCircle(SpawnRadius);
			SpawnLocation = Bus->GetCheckLocation() + FVector(RandomOffset.X, RandomOffset.Y, 0.0f);
		}
		else
		{
			continue;
		}

		if (auto* Passenger = GetWorld()->SpawnActor<ANPCCharacter>(PassengerClass, SpawnLocation, SpawnRotation, SpawnParams))
		{
			PassengersQueue.Enqueue(Passenger);
		}
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

