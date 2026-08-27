#include "Bus.h"
#include "Components/SplineComponent.h"
#include "Components/ArrowComponent.h"
#include "BusSeat.h"
#include "BrackeysJam26/Components/BusQueueComponent.h"
#include "BrackeysJam26/Inspectable/IDActor.h"
#include "BrackeysJam26/Inspectable/TicketActor.h"
#include "BusRouteManager.h"
#include <Kismet/GameplayStatics.h>

ABus::ABus()
{
	PrimaryActorTick.bCanEverTick = false;

	auto* pRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = pRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	RoofMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Roof"));
	RoofMesh->SetupAttachment(Mesh);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);

	CheckLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("CheckLocation"));
	CheckLocation->SetupAttachment(RootComponent);
	ExitLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("ExitLocation"));
	ExitLocation->SetupAttachment(RootComponent);
	DespawnLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("DespawnLocation"));
	DespawnLocation->SetupAttachment(RootComponent);

	BusQueue = CreateDefaultSubobject<UBusQueueComponent>(TEXT("BusQueue"));
	BusQueue->OnQueueEmpty.AddDynamic(this, &ABus::CloseDoors);
}

void ABus::BeginPlay()
{
	Super::BeginPlay();
	
	SetPassengerDocsVisibility(false);

	OnOpenDoors.Broadcast();

	auto* BusManager = Cast<ABusRouteManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABusRouteManager::StaticClass()));
	if (BusManager)
	{
		BusManager->OnArrive.AddDynamic(this, &ABus::OpenDoors);
	}
}

//void ABus::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

FVector ABus::GetCheckLocation() const
{
	return CheckLocation->GetComponentLocation();
}

FVector ABus::GetExitLocation() const
{
	return ExitLocation->GetComponentLocation();
}

FVector ABus::GetDespawnLocation() const
{
	return DespawnLocation->GetComponentLocation();
}

ABusSeat* ABus::GetAvailableSeat() const
{
	for (ABusSeat* Seat : Seats)
	{
		if (Seat && !Seat->IsOccupied())
		{
			return Seat;
		}
	}

	return nullptr;
}

void ABus::SetRoofVisibility(bool visible)
{
	RoofMesh->SetVisibility(visible);
	RoofMesh->SetCollisionEnabled(visible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

void ABus::SetPassengerDocsVisibility(bool bVisible)
{
	if (IDActor)
	{
		IDActor->SetActorHiddenInGame(!bVisible);
		IDActor->SetActorEnableCollision(bVisible);
	}

	if (TicketActor)
	{
		TicketActor->SetActorHiddenInGame(!bVisible);
		TicketActor->SetActorEnableCollision(bVisible);
	}
}

void ABus::UpdatePassengerDocs(const FPassengerRecord& Record)
{
	if (IDActor)
	{
		IDActor->UpdateID(Record.PresentedPassport);
	}

	if (TicketActor)
	{
		TicketActor->UpdateTicket(Record.PresentedTicket);
	}
}

void ABus::OpenDoors()
{
	OnOpenDoors.Broadcast();
}

void ABus::CloseDoors()
{
	OnCloseDoors.Broadcast();
}

